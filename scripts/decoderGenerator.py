from pprint import pprint

opCodeSlices = set()

def checkAmbiguities(opCodes):
    pass

def longestNonWildcardSubsequence(opCodes, mask=None):
    """ Returns the start and end index of the longest substring containing
    only wildcard characters ('*') """
    # Invalid values for start and end
    start = 1
    end = 0
    if not mask:
        mask = "1"*len(opCodes[0])
    for x in range(len(mask)):
        for y in range(x, len(mask)):
            rangeGood = True
            for op in opCodes:
                for i in range(x, y+1):
                    if op[i] == "*" or mask[i] == "*":
                        rangeGood = False
                        break
                if not rangeGood:
                    break
            if rangeGood:
                if y-x > end-start:
                    end = y
                    start = x

    mask = [ c for c in mask ]

    for i in range(start, end+1):
        mask[i] = "*"

    return start, end, ''.join(mask)

def partition(opCodes, start, end):
    """ Split a list into smaller lists based on the substring between start and
    end.

    opCodes - input list
    start   - index of the start of the substring
    end     - index of the end of the substring (inclusive)

    return  - a dict with the different values of substrings as keys, and values
              which are strings in the input list matching the substring """

    d = {}

    length = len(opCodes[0])
    prefix = "*"*(start)
    fix = "*"*(end-start+1)
    postfix = "*"*(length-end-1)

    for op in opCodes:
        d.setdefault( prefix + op[start:end+1] + postfix, [] )
        d[ prefix + op[start:end+1] + postfix ].append(  op[0:start] + fix + op[end+1:]  )

    return d

def partitionFromMask(mask):
    """ Return the start and end address of the first substring without
    wildcards """
    for i in range(len(mask)):
        if mask[i] == '*':
            continue
        for j in range(i+1, len(mask)):
            if mask[j] == '*':
                break
        else:
            if i+1 == len(mask):
                j = i+1
            else:
                j += 1
        break
    return i, (j-1)

def subdivide(opCodes):
    """ Recursively parition a list of strings based on the longest substring
    containing no wildcards. """
    a, b, mask = longestNonWildcardSubsequence(opCodes)
    if b-a+1 < 1:
        return opCodes
    d = partition(opCodes, a, b)
    for key, value in d.items():
        if len(value) > 1:
            d[key] = subdivide(value)

    return d

def match(buildup, opCodes):
    """ Return a value in opCodes where the key matches buildup """
    if buildup in opCodes:
        return opCodes[buildup]
    else:
        # Looks like we'll need to do some more intelligent matching...
        keys = opCodes.keys()
        for i in range(len(buildup)):
            keys = [ k for k in keys if k[i] == buildup[i] or buildup[i] == '*' ]
        if len(keys) == 1:
            return opCodes[keys[0]]
        else:
            return "/*!!!!!!! ERROR creating tree, keys: %s !!!!!!*/" % keys

def combine(a, b):
    """ Takes 2 strings of the same length and returns a string 's' where
    s[i] = a[i] if a[i] != '*' else b[i]

    i.e. it removes wildcards from 'a' by replacing them with the characters
         in the same position in 'b' 

    returns string with the above rule"""

    assert len(a) == len(b)
    combination = []
    for i in range(len(a)):
        assert a[i] == '*' or b[i] == '*', "%s: %s %s" % (i, a, b)
        if a[i] != '*':
            combination.append(a[i])
        else:
            combination.append(b[i])
    return ''.join(combination)

def traverse(d, leafMap, buildup=None):
    """ Return a decode tree with the leaves replaced with the value
    corresponding to the edges traversed in the tree to get to the leaf.

    d       - the decode tree to modify
    leafMap - the map between the path to get to the leaf, and the desired
              leaf value

    return  - the decode tree with the leaves modified"""

    if not buildup:
        buildup = "*"*len( d.keys()[0] )
    for key, value in d.items():
        b2 = combine(buildup, key)
        if isinstance(value, dict):
            traverse(d[key], leafMap, b2)
        else:
            d[key] = match(b2, leafMap)
    return d

def getDecodeTree(opCodes, wildcardList, ignoreList):
    """ Return a nested dict for op code decoding. The keys of the nested dict
    act as edges for a tree. '*' characters in the key represent digits that
    are ignored from the op code while following that edge. Each leaf of the
    tree is the opCode resulting for the input defined by the edges of the
    tree.

    Results for ambiguous input are undefined.

    opCodes      - a list of op codes which need to be trasnformed into a decode
                   tree
    wildcardList - a list of characters which are replaced in the op codes with
                   and asterisk
    ignoreList   - a list of characters which are removed from the op code
                   defintion for creating the decode tree

    return       - a nested dict which forms a decode tree"""

    ogOpCodes = opCodes

    # Simplify op codes using the provided ignore and wild card lists
    for c in ignoreList:
        opCodes = [ s.replace(c, "") for s in opCodes ]

    for c in wildcardList:
        opCodes = [ s.replace(c, "*") for s in opCodes ]

    # Map the simplified op codes to the provided op codes
    newToOldOpCodes = {}
    for new, old in zip(opCodes, ogOpCodes):
        newToOldOpCodes[new] = old

    return traverse(subdivide(newToOldOpCodes.keys()), newToOldOpCodes)

def getSwitch(gen, default, tree, tabdepth, base=2):
    """ Return a C-style switch statement which represents the decode tree.

    gen         - a function which returns a string based on the value of a
                  leaf
    default     - a string for default cases in the switch statement
    tree        - the decode tree to be transformed into a nested switch
                  statement
    tabdepth    - amount of indentation for the generated switch statement

    return      - a string of a C-style switch statement"""

    out = ""
    tabs = '\t'*tabdepth

    if not isinstance(tree, dict):
        # We are at a leaf node
        out = gen(tree)
        out = out.replace("\n", "\n"+tabs)
    else:
        items = sorted(tree.items())
        subsequence = partitionFromMask(items[0][0])
        opCodeSlices.add(subsequence)
        out += "switch (op.digit%s_%s) {\n" % subsequence
        for key, value in items:
            out += tabs + "case 0x%s: " % format(int(key[subsequence[0]:subsequence[1]+1], base), 'x').upper()
            out += getSwitch( gen, default, tree[key], tabdepth+1, base )
            out += " break;\n"
        out += tabs + "default: %s break;\n%s}" % (default, tabs)

    return out

def genDisassemble(tree):
    return "std::cout << \"%s\" << std::endl;" % opCodes[tree][1]
