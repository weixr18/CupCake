from copy import deepcopy
import re

Punctuators = [
    ",", "(", ")", "[", "]",
    ";", "{", "}", ":", "?", "="
]
Terminators = []
Non_Terminators = []
Structure_lines = []
NT_INFO = {}
LL1_TABLE = {}

def print_sl():
    with open("./info/parser/grammar-expressions.txt", 'w') as f:
        for sl in Structure_lines:
            f.write(f'{sl["id"]} {sl["left"]} := ')
            for i in range(sl["count"]):
                f.write(f'{sl["right"][i]} ')
            f.write("\n")

            # if sl["right"][0] == sl["left"]:
            #     print(f'{sl["id"]} {sl["left"]} := ', end="")
            #     for i in range(sl["count"]):
            #         print(f'{sl["right"][i]} ', end="")
            #     print("")
        pass
    pass

def print_term_and_nonterms():
    print("Terminators:")
    tmnts = deepcopy(Terminators)
    tmnts = sorted(tmnts, key=len)
    for t in tmnts:
        print(t, end=" ")
    print("")
    
    print("Non-terminators:")
    nonts = deepcopy(Non_Terminators)
    nonts = sorted(nonts, key=len)
    for nt in nonts:
        print(nt, end=" ")
    print("")
    
    pass
    

def get_expressions():
    lines = []
    with open("D:\\Codes\\CXX\\Cupcake\\info\\parser\\grammar.txt", "r") as f:
        lines = f.readlines()
    new_lines = []
    for line in lines:
        line = line.replace('\n', '').replace('\r', '')
        if ":=" in line:
            new_lines.append(line)
    return new_lines

def split_expressions(lines):
    split_lines = []
    for line in lines:
        left, right = line.split(':=')
        left = left.replace(' ', '')
        Non_Terminators.append(left)
        sub_rights = right.split('|')
        for s in sub_rights:
            sub_exp = left + ":=" + s
            split_lines.append(sub_exp)
        pass
    print("Lines get.")
    return split_lines

def get_terminators(lines):
    global Terminators
    global Non_Terminators
    for line in lines:
        ts = re.findall(r'<[a-z_]*>', line)
        for t in ts:
            Terminators.append(t)
    Terminators = list(set(Terminators))
    
    for p in Punctuators:
        Terminators.append(p)
    print("Terminators get.")
    pass

def get_structure_lines(lines):
    global Terminators
    global Non_Terminators

    words = []
    for t in Non_Terminators:
        words.append(t)
    for t in Terminators:
        words.append(t)
    words = sorted(words, key=len)[::-1]

    id = 0
    s_lines = []
    for line in lines:
        s_line = {
            "id": 0,
            "left": "",
            "right": [],
            "count":0,
        }
        left, right = line.split(':=')
        s_line["left"] = left
        s_line["id"] = id
        id = id+1

        line_len = len(right)
        cur_pos = 0
        while(cur_pos != line_len):
            if(right[cur_pos] == ' '):
                cur_pos += 1
                continue
            for w in words:
                l = len(w)
                if(cur_pos+ l > line_len):
                    continue
                if(right[cur_pos:cur_pos+l] == w):
                    cur_pos += l
                    s_line["count"] += 1
                    s_line["right"].append(w)
                pass
            pass
        # print(s_line["id"], s_line["left"], ":=", s_line["right"])
        s_lines.append(s_line)
    print("Structure lines get.")
    return s_lines

def calc_first_set():
    done_nts = set()
    done_sl_ids = set()
    while(len(done_nts) < len(Non_Terminators)):
        for nt in Non_Terminators:
            if nt in done_nts:
                continue
            for sl in Structure_lines:
                if sl["left"] != nt:
                    continue
                if sl["id"] in done_sl_ids:
                    continue
                if(sl["right"][0] in Terminators):
                    NT_INFO[nt]["first"].add(sl["right"][0])
                    if sl["right"][0] not in NT_INFO[nt]["first-add"]:
                        NT_INFO[nt]["first-add"][sl["right"][0]] = []
                    NT_INFO[nt]["first-add"][sl["right"][0]].append(sl["id"])
                    done_sl_ids.add(sl["id"])
                    continue
                else:
                    # sl["right"][0] in Non_Terminators
                    cur_id = 0
                    first_set = set()
                    sl_done = False

                    # "A:=AB" type sl
                    if sl["right"][0] == nt:
                        # find "other sls"
                        other_sls = []
                        for ssl in Structure_lines:
                            if ssl["left"] != nt:
                                continue
                            if ssl["right"][0] == nt:
                                continue
                            other_sls.append(ssl)
                        # check if they are all ready. if not, don't calculate.
                        all_ready = True
                        for ssl in other_sls:
                            if ssl["id"] not in done_sl_ids:
                                all_ready = False
                                break
                        if not all_ready:
                            continue
                        first_set = NT_INFO[nt]["first"] # passive first set
                        # if <none> not in first
                        if "<none>" not in NT_INFO[nt]["first"]:
                            cur_id = sl["count"] # no need to see after words
                        pass # end if "A:=AB" type
                    
                    while(cur_id < sl["count"]):
                        cur_word = sl["right"][cur_id]
                        if cur_word == nt:
                            # just jump this one.
                            cur_id += 1
                            continue
                        if cur_word in Terminators:
                            # no more possibilities. end loop.
                            first_set = first_set | set([cur_word])
                            sl_done = True
                            break
                        else:
                            # cur_word is non-terminator
                            if cur_word in done_nts:
                                # cur_word's first set is done
                                tmp = NT_INFO[cur_word]["first"]
                                first_set = first_set | tmp
                                if "<none>" in tmp:
                                    # still posibilities. keep calculating
                                    cur_id += 1
                                else:
                                    sl_done = True
                                    break
                            else:
                                # cur_word's first set is not ok, cannot calculate.
                                break
                            pass # end cur_word is non-terminator
                        pass # sl["right"] search loop
                    if sl_done or cur_id == sl["count"]:
                        NT_INFO[nt]["first"] = NT_INFO[nt]["first"] | first_set
                        for w in first_set:
                            if w not in NT_INFO[nt]["first-add"]:
                                NT_INFO[nt]["first-add"][w] = []
                            NT_INFO[nt]["first-add"][w].append(sl["id"])
                        done_sl_ids.add(sl["id"])
                        continue
                pass  # sl traverse loop
            
            # check nt done
            if nt not in done_nts:
                nt_done = True
                for sl in Structure_lines:
                    if sl["left"] == nt and sl["id"] not in done_sl_ids:
                        nt_done = False
                if(nt_done):
                    # print(len(done_nts), nt, "first set:", NT_INFO[nt]["first"])
                    done_nts.add(nt)
            pass # nt loop
        pass # big loop
    print("First set calculated.")
    pass # end calc function

def calc_follow_set():

    NT_INFO["CODE"]["follow"].add("<eof>") # special terminator
    done_nts = set()
    while(len(done_nts) < len(Non_Terminators)):
        for nt in (set(Non_Terminators) - done_nts):
            tmp = deepcopy(NT_INFO[nt]["right-sl"])
            for id in NT_INFO[nt]["right-sl"]:
                sl = Structure_lines[id]
                sl_done = True
                for i in range(sl["count"]):
                    if(sl["right"][i] != nt):
                        continue
                    if i == sl["count"] - 1:
                        # end of sl, look for left's follow set
                        left_wd = sl["left"]
                        if left_wd == nt:
                            # depend on itself is nonsence. just ignore.
                            break
                        if(left_wd in done_nts):
                            NT_INFO[nt]["follow"] = NT_INFO[nt]["follow"] | NT_INFO[left_wd]["follow"]
                            for t in NT_INFO[left_wd]["follow"]:
                                if t not in NT_INFO[nt]["follow-add"]:
                                    NT_INFO[nt]["follow-add"][t] = []
                                NT_INFO[nt]["follow-add"][t].append(id)
                        else:
                            sl_done = False
                        # no other possibilities.
                        break

                    cur_pos = i+1
                    while(cur_pos < sl["count"]):
                        cur_word = sl["right"][cur_pos]
                        if cur_word == nt:
                            # wait for next hit
                            break
                        if cur_word in Terminators:
                            # add the terminator, done.
                            NT_INFO[nt]["follow"].add(cur_word)
                            if cur_word not in NT_INFO[nt]["follow-add"]:
                                NT_INFO[nt]["follow-add"][cur_word] = []
                            NT_INFO[nt]["follow-add"][cur_word].append(sl["id"])
                            break
                        else:
                            # add the first set
                            NT_INFO[nt]["follow"] = NT_INFO[nt]["follow"] | NT_INFO[cur_word]["first"]
                            for t in NT_INFO[cur_word]["first"]:
                                if t not in NT_INFO[nt]["follow-add"]:
                                    NT_INFO[nt]["follow-add"][t] = []
                                NT_INFO[nt]["follow-add"][t].append(id)
                            if "<none>" not in NT_INFO[cur_word]["first"]:
                                # no more possibilities
                                break
                            else:
                                cur_pos += 1
                        pass # middle search loop
                    pass # sl right hand search loop

                if sl_done:
                    tmp.remove(id)
                pass # nt presence search loop
            NT_INFO[nt]["right-sl"] = deepcopy(tmp)
            pass # nt loop

        # nt check
        for nt in (set(Non_Terminators) - done_nts):
            if(len(NT_INFO[nt]["right-sl"]) == 0):
                done_nts.add(nt)
                # print(len(done_nts), nt, NT_INFO[nt]["follow"])
        
        pass # big loop
    print("Follow set calculated.")
    pass # calc func

def calc_ll1_table():
    # LL1 table grid: (<non-term>, [<term>|<eof>], grammar_id)

    # first set
    for nt in Non_Terminators:
        for t in NT_INFO[nt]["first"]:
            if t == "<none>":
                continue
            sl_ids = NT_INFO[nt]["first-add"][t]
            if t not in LL1_TABLE.keys():
                LL1_TABLE[t] = set()
            for sl_id in sl_ids:
                LL1_TABLE[t].add((nt, sl_id))
    
    # follow set
    for sl in Structure_lines:
        add_follow = True
        for word in sl["right"]:
            if word == "<none>":
                break
            if word in Terminators and word != "<none>":
                add_follow = False
                break
            elif word in Non_Terminators:
                if "<none>" not in NT_INFO[word]["first"]:
                    add_follow = False
                    break
                pass
            pass

        if add_follow:
            nt = sl["left"]
            for t in NT_INFO[nt]["follow"]:
                if t == "<none>":
                    continue
                if t not in LL1_TABLE.keys():
                    LL1_TABLE[t] = set()
                LL1_TABLE[t].add((nt, sl["id"]))
                # sl_ids = NT_INFO[nt]["follow-add"][t]
                # for sl_id in sl_ids:
                #     LL1_TABLE[t].add((nt, sl_id))
            pass
        pass

    print("LL1 table calculated.")
    pass

def generate_cpp():
    sum = 0
    for item in LL1_TABLE:
        sum += len(item)
    print("Terminator count:", len(Terminators))
    print("Non-Terminator count:", len(Non_Terminators))
    print("Grammar expression count:", len(Structure_lines))
    print("LL(1) table count:", sum)

    with open("./src/ll1_table.cpp", 'w') as f:
        # include
        f.writelines([
            "#include <stdlib.h>\n",
            "#include <vector>\n",
            "\n",
            '#include "utils.h"\n',
            '#include "parser.h"\n',
            "\n",
        ])

        # counts
        f.write(f"int TermCount = {len(Terminators)};\n")
        f.write(f"int NonTermCount = {len(Non_Terminators)};\n\n")

        # non-terminators
        f.write("std::vector<CUP_NONT> NonTerminators = {")
        count = 0
        for nt in Non_Terminators:
            if count % 5 == 0:
                f.write('\n    ')
            f.write(f'"{nt}", ')
            count += 1
        f.write("\n};\n\n")

        # right hand sides
        for sl in Structure_lines:
            sl_id = sl["id"]
            f.write(f"const char* right_{sl_id}[] = ")
            f.write("{")
            for w in sl["right"]:
                f.write(f'"{w}", ')
            f.write("};\n")
        f.write("\n")
        
        # exprs
        f.write('std::vector<CUP_GRAMMAR_EXPR> GrammarExprs = {\n')
        for sl in Structure_lines:
            sl_id = sl["id"]
            sl_left = sl["left"]
            sl_count = sl["count"]
            f.write('    (CUP_GRAMMAR_EXPR){')
            f.write(f'{sl_id}, "{sl_left}", right_{sl_id}, {sl_count}')
            f.write('},\n')
        f.write('};\n\n')

        # LL(1) table item
        count = 0
        for t in LL1_TABLE.keys():
            f.write(f'CUP_LT_ITEMS item_{count} = '+'{\n')
            for item in LL1_TABLE[t]:
                nt, id_ = item
                f.write('    {' + f'"{nt}", {id_}' + '},\n')
            f.write('};\n')
            count += 1
        f.write('\n')

        # LL(1) table
        count = 0
        f.write('CUP_LT LL1Table = {\n')
        for t in LL1_TABLE.keys():
            f.write('    {' + f'"{t}", item_{count}' + '},\n')
            count += 1
        f.write('};\n')        
    pass

if __name__ == "__main__":
    lines = get_expressions()
    lines = split_expressions(lines)
    get_terminators(lines)
    Structure_lines = get_structure_lines(lines)

    for nt in Non_Terminators:
        info = {
            "first": set(),
            "follow": set(),
            "right-sl": set(),
            "first-add": {}, # addition message of first set to generate LL(1) table
            "follow-add": {}, # addition message of follow set to generate LL(1) table
        }
        NT_INFO[nt] = info
    for sl in Structure_lines:
        for w in sl["right"]:
            if w in Non_Terminators:
                NT_INFO[w]["right-sl"].add(sl["id"])
    
    # print_term_and_nonterms()
    print_sl()

    calc_first_set()
    calc_follow_set()
    calc_ll1_table()
    generate_cpp()
    pass