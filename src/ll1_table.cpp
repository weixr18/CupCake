#include <stdlib.h>
#include <vector>

#include "utils.h"
#include "parser.h"

int TermCount = 31;
int NonTermCount = 47;

std::vector<CUP_NONT> NonTerminators = {
    "CODE", "CODEPART", "FUNC_DEF", "FUNC_STMT", "VAR_DEF", 
    "VAR_STMT", "ARRAY_DEF", "VAR_IDENTIFIERS", "FUNC_IDENTIFIER", "VAR_IDENTIFIER", 
    "VAR_IDENTIFIER_A", "PARAMETERS_N", "PARAMETERS", "PARAMETER", "PARAMETER_IDF", 
    "BLOCK", "S_OR_B", "SENTENCES_N", "SENTENCES", "SENTENCE", 
    "FOR_STC", "FOR_EXPRS", "EXPR_AN", "WHILE_STC", "DO_STC", 
    "BREAK_STC", "CONTINUE_STC", "IF_STC", "ELSE_STC_N", "ELSE_STC", 
    "SWITCH_STC", "CS_OR_D", "CASES_N", "CASES", "CASE_STC", 
    "DEFAULT_STC", "RETURN_STC", "EXPR_N", "ASSIGN_STC", "ASSIGN_EXP", 
    "ARRAY_VISIT", "EXPR", "EXPR_", "COMMA_EXP", "COMMA_EXP_ELE", 
    "FUNC_CALL", "TYPE", 
};

const char* right_0[] = {"CODEPART", };
const char* right_1[] = {"CODE", "CODEPART", };
const char* right_2[] = {"FUNC_DEF", };
const char* right_3[] = {"FUNC_STMT", };
const char* right_4[] = {"VAR_DEF", };
const char* right_5[] = {"VAR_STMT", };
const char* right_6[] = {"ARRAY_DEF", };
const char* right_7[] = {"<type>", "FUNC_IDENTIFIER", "(", "PARAMETERS_N", ")", "BLOCK", };
const char* right_8[] = {"<type>", "FUNC_IDENTIFIER", "(", "PARAMETERS_N", ")", ";", };
const char* right_9[] = {"<type>", "VAR_IDENTIFIER", "=", "EXPR", ";", };
const char* right_10[] = {"<type>", "VAR_IDENTIFIERS", ";", };
const char* right_11[] = {"<type>", "VAR_IDENTIFIER_A", ";", };
const char* right_12[] = {"<type>", "VAR_IDENTIFIER_A", "=", "{", "COMMA_EXP", "}", ";", };
const char* right_13[] = {"VAR_IDENTIFIER", };
const char* right_14[] = {"VAR_IDENTIFIERS", ",", "VAR_IDENTIFIER", };
const char* right_15[] = {"<identifier>", };
const char* right_16[] = {"<identifier>", };
const char* right_17[] = {"<identifier>", };
const char* right_18[] = {"VAR_IDENTIFIER_A", "[", "<number>", "]", };
const char* right_19[] = {"PARAMETERS", };
const char* right_20[] = {"<none>", };
const char* right_21[] = {"PARAMETER", };
const char* right_22[] = {"PARAMETERS", ",", "PARAMETER", };
const char* right_23[] = {"<type>", "<identifier>", };
const char* right_24[] = {"<type>", "(", "PARAMETER_IDF", ")", };
const char* right_25[] = {"<identifier>", };
const char* right_26[] = {"PARAMETER_IDF", "[", "<number>", "]", };
const char* right_27[] = {"{", "SENTENCES_N", "}", };
const char* right_28[] = {"SENTENCE", };
const char* right_29[] = {"BLOCK", };
const char* right_30[] = {"SENTENCES", };
const char* right_31[] = {"<none>", };
const char* right_32[] = {"SENTENCE", };
const char* right_33[] = {"SENTENCES", "SENTENCE", };
const char* right_34[] = {"EXPR", ";", };
const char* right_35[] = {"ASSIGN_STC", };
const char* right_36[] = {"FOR_STC", };
const char* right_37[] = {"WHILE_STC", };
const char* right_38[] = {"DO_STC", };
const char* right_39[] = {"IF_STC", };
const char* right_40[] = {"SWITCH_STC", };
const char* right_41[] = {"BREAK_STC", };
const char* right_42[] = {"CONTINUE_STC", };
const char* right_43[] = {"VAR_STMT", };
const char* right_44[] = {"VAR_DEF", };
const char* right_45[] = {"ARRAY_DEF", };
const char* right_46[] = {"RETURN_STC", };
const char* right_47[] = {"<for>", "(", "FOR_EXPRS", ")", "BLOCK", };
const char* right_48[] = {"EXPR_AN", ";", "EXPR_AN", ";", "EXPR_AN", };
const char* right_49[] = {"ASSIGN_EXP", };
const char* right_50[] = {"EXPR", };
const char* right_51[] = {"<none>", };
const char* right_52[] = {"<while>", "(", "EXPR", ")", "BLOCK", };
const char* right_53[] = {"<do>", "S_OR_B", "<while>", "(", "EXPR", ")", };
const char* right_54[] = {"<break>", ";", };
const char* right_55[] = {"<continue>", ";", };
const char* right_56[] = {"<if>", "(", "EXPR", ")", "S_OR_B", "ELSE_STC_N", };
const char* right_57[] = {"ELSE_STC", };
const char* right_58[] = {"<none>", };
const char* right_59[] = {"<else>", "BLOCK", };
const char* right_60[] = {"<switch>", "(", "EXPR", ")", "{", "CS_OR_D", "}", };
const char* right_61[] = {"CASES_N", };
const char* right_62[] = {"CASES_N", "DEFAULT_STC", };
const char* right_63[] = {"CASES", };
const char* right_64[] = {"<none>", };
const char* right_65[] = {"CASE_STC", };
const char* right_66[] = {"CASES", "CASE_STC", };
const char* right_67[] = {"<case>", "EXPR", ":", "S_OR_B", };
const char* right_68[] = {"<default>", ":", "S_OR_B", };
const char* right_69[] = {"<return>", "EXPR_N", ";", };
const char* right_70[] = {"EXPR", };
const char* right_71[] = {"<none>", };
const char* right_72[] = {"ASSIGN_EXP", ";", };
const char* right_73[] = {"<identifier>", "=", "EXPR", };
const char* right_74[] = {"(", "ARRAY_VISIT", ")", "=", "EXPR", };
const char* right_75[] = {"<identifier>", };
const char* right_76[] = {"ARRAY_VISIT", "[", "EXPR", "]", };
const char* right_77[] = {"EXPR_", };
const char* right_78[] = {"EXPR", "<opr_right>", };
const char* right_79[] = {"EXPR", "<opr_two>", "EXPR", };
const char* right_80[] = {"EXPR", "<opr_minus>", "EXPR", };
const char* right_81[] = {"<opr_left>", "EXPR", };
const char* right_82[] = {"<identifier>", };
const char* right_83[] = {"<number>", };
const char* right_84[] = {"<sizeof>", "(", "TYPE", ")", };
const char* right_85[] = {"<opr_minus>", "<number>", };
const char* right_86[] = {"(", "EXPR", ")", };
const char* right_87[] = {"ARRAY_VISIT", };
const char* right_88[] = {"FUNC_CALL", };
const char* right_89[] = {"COMMA_EXP_ELE", };
const char* right_90[] = {"COMMA_EXP", ",", "COMMA_EXP_ELE", };
const char* right_91[] = {"EXPR_", };
const char* right_92[] = {"(", "EXPR", ")", };
const char* right_93[] = {"<identifier>", "(", "COMMA_EXP", ")", };
const char* right_94[] = {"<type>", };

std::vector<CUP_GRAMMAR_EXPR> GrammarExprs = {
    (CUP_GRAMMAR_EXPR){0, "CODE", right_0, 1},
    (CUP_GRAMMAR_EXPR){1, "CODE", right_1, 2},
    (CUP_GRAMMAR_EXPR){2, "CODEPART", right_2, 1},
    (CUP_GRAMMAR_EXPR){3, "CODEPART", right_3, 1},
    (CUP_GRAMMAR_EXPR){4, "CODEPART", right_4, 1},
    (CUP_GRAMMAR_EXPR){5, "CODEPART", right_5, 1},
    (CUP_GRAMMAR_EXPR){6, "CODEPART", right_6, 1},
    (CUP_GRAMMAR_EXPR){7, "FUNC_DEF", right_7, 6},
    (CUP_GRAMMAR_EXPR){8, "FUNC_STMT", right_8, 6},
    (CUP_GRAMMAR_EXPR){9, "VAR_DEF", right_9, 5},
    (CUP_GRAMMAR_EXPR){10, "VAR_STMT", right_10, 3},
    (CUP_GRAMMAR_EXPR){11, "VAR_STMT", right_11, 3},
    (CUP_GRAMMAR_EXPR){12, "ARRAY_DEF", right_12, 7},
    (CUP_GRAMMAR_EXPR){13, "VAR_IDENTIFIERS", right_13, 1},
    (CUP_GRAMMAR_EXPR){14, "VAR_IDENTIFIERS", right_14, 3},
    (CUP_GRAMMAR_EXPR){15, "FUNC_IDENTIFIER", right_15, 1},
    (CUP_GRAMMAR_EXPR){16, "VAR_IDENTIFIER", right_16, 1},
    (CUP_GRAMMAR_EXPR){17, "VAR_IDENTIFIER_A", right_17, 1},
    (CUP_GRAMMAR_EXPR){18, "VAR_IDENTIFIER_A", right_18, 4},
    (CUP_GRAMMAR_EXPR){19, "PARAMETERS_N", right_19, 1},
    (CUP_GRAMMAR_EXPR){20, "PARAMETERS_N", right_20, 1},
    (CUP_GRAMMAR_EXPR){21, "PARAMETERS", right_21, 1},
    (CUP_GRAMMAR_EXPR){22, "PARAMETERS", right_22, 3},
    (CUP_GRAMMAR_EXPR){23, "PARAMETER", right_23, 2},
    (CUP_GRAMMAR_EXPR){24, "PARAMETER", right_24, 4},
    (CUP_GRAMMAR_EXPR){25, "PARAMETER_IDF", right_25, 1},
    (CUP_GRAMMAR_EXPR){26, "PARAMETER_IDF", right_26, 4},
    (CUP_GRAMMAR_EXPR){27, "BLOCK", right_27, 3},
    (CUP_GRAMMAR_EXPR){28, "S_OR_B", right_28, 1},
    (CUP_GRAMMAR_EXPR){29, "S_OR_B", right_29, 1},
    (CUP_GRAMMAR_EXPR){30, "SENTENCES_N", right_30, 1},
    (CUP_GRAMMAR_EXPR){31, "SENTENCES_N", right_31, 1},
    (CUP_GRAMMAR_EXPR){32, "SENTENCES", right_32, 1},
    (CUP_GRAMMAR_EXPR){33, "SENTENCES", right_33, 2},
    (CUP_GRAMMAR_EXPR){34, "SENTENCE", right_34, 2},
    (CUP_GRAMMAR_EXPR){35, "SENTENCE", right_35, 1},
    (CUP_GRAMMAR_EXPR){36, "SENTENCE", right_36, 1},
    (CUP_GRAMMAR_EXPR){37, "SENTENCE", right_37, 1},
    (CUP_GRAMMAR_EXPR){38, "SENTENCE", right_38, 1},
    (CUP_GRAMMAR_EXPR){39, "SENTENCE", right_39, 1},
    (CUP_GRAMMAR_EXPR){40, "SENTENCE", right_40, 1},
    (CUP_GRAMMAR_EXPR){41, "SENTENCE", right_41, 1},
    (CUP_GRAMMAR_EXPR){42, "SENTENCE", right_42, 1},
    (CUP_GRAMMAR_EXPR){43, "SENTENCE", right_43, 1},
    (CUP_GRAMMAR_EXPR){44, "SENTENCE", right_44, 1},
    (CUP_GRAMMAR_EXPR){45, "SENTENCE", right_45, 1},
    (CUP_GRAMMAR_EXPR){46, "SENTENCE", right_46, 1},
    (CUP_GRAMMAR_EXPR){47, "FOR_STC", right_47, 5},
    (CUP_GRAMMAR_EXPR){48, "FOR_EXPRS", right_48, 5},
    (CUP_GRAMMAR_EXPR){49, "EXPR_AN", right_49, 1},
    (CUP_GRAMMAR_EXPR){50, "EXPR_AN", right_50, 1},
    (CUP_GRAMMAR_EXPR){51, "EXPR_AN", right_51, 1},
    (CUP_GRAMMAR_EXPR){52, "WHILE_STC", right_52, 5},
    (CUP_GRAMMAR_EXPR){53, "DO_STC", right_53, 6},
    (CUP_GRAMMAR_EXPR){54, "BREAK_STC", right_54, 2},
    (CUP_GRAMMAR_EXPR){55, "CONTINUE_STC", right_55, 2},
    (CUP_GRAMMAR_EXPR){56, "IF_STC", right_56, 6},
    (CUP_GRAMMAR_EXPR){57, "ELSE_STC_N", right_57, 1},
    (CUP_GRAMMAR_EXPR){58, "ELSE_STC_N", right_58, 1},
    (CUP_GRAMMAR_EXPR){59, "ELSE_STC", right_59, 2},
    (CUP_GRAMMAR_EXPR){60, "SWITCH_STC", right_60, 7},
    (CUP_GRAMMAR_EXPR){61, "CS_OR_D", right_61, 1},
    (CUP_GRAMMAR_EXPR){62, "CS_OR_D", right_62, 2},
    (CUP_GRAMMAR_EXPR){63, "CASES_N", right_63, 1},
    (CUP_GRAMMAR_EXPR){64, "CASES_N", right_64, 1},
    (CUP_GRAMMAR_EXPR){65, "CASES", right_65, 1},
    (CUP_GRAMMAR_EXPR){66, "CASES", right_66, 2},
    (CUP_GRAMMAR_EXPR){67, "CASE_STC", right_67, 4},
    (CUP_GRAMMAR_EXPR){68, "DEFAULT_STC", right_68, 3},
    (CUP_GRAMMAR_EXPR){69, "RETURN_STC", right_69, 3},
    (CUP_GRAMMAR_EXPR){70, "EXPR_N", right_70, 1},
    (CUP_GRAMMAR_EXPR){71, "EXPR_N", right_71, 1},
    (CUP_GRAMMAR_EXPR){72, "ASSIGN_STC", right_72, 2},
    (CUP_GRAMMAR_EXPR){73, "ASSIGN_EXP", right_73, 3},
    (CUP_GRAMMAR_EXPR){74, "ASSIGN_EXP", right_74, 5},
    (CUP_GRAMMAR_EXPR){75, "ARRAY_VISIT", right_75, 1},
    (CUP_GRAMMAR_EXPR){76, "ARRAY_VISIT", right_76, 4},
    (CUP_GRAMMAR_EXPR){77, "EXPR", right_77, 1},
    (CUP_GRAMMAR_EXPR){78, "EXPR", right_78, 2},
    (CUP_GRAMMAR_EXPR){79, "EXPR", right_79, 3},
    (CUP_GRAMMAR_EXPR){80, "EXPR", right_80, 3},
    (CUP_GRAMMAR_EXPR){81, "EXPR", right_81, 2},
    (CUP_GRAMMAR_EXPR){82, "EXPR_", right_82, 1},
    (CUP_GRAMMAR_EXPR){83, "EXPR_", right_83, 1},
    (CUP_GRAMMAR_EXPR){84, "EXPR_", right_84, 4},
    (CUP_GRAMMAR_EXPR){85, "EXPR_", right_85, 2},
    (CUP_GRAMMAR_EXPR){86, "EXPR_", right_86, 3},
    (CUP_GRAMMAR_EXPR){87, "EXPR_", right_87, 1},
    (CUP_GRAMMAR_EXPR){88, "EXPR_", right_88, 1},
    (CUP_GRAMMAR_EXPR){89, "COMMA_EXP", right_89, 1},
    (CUP_GRAMMAR_EXPR){90, "COMMA_EXP", right_90, 3},
    (CUP_GRAMMAR_EXPR){91, "COMMA_EXP_ELE", right_91, 1},
    (CUP_GRAMMAR_EXPR){92, "COMMA_EXP_ELE", right_92, 3},
    (CUP_GRAMMAR_EXPR){93, "FUNC_CALL", right_93, 4},
    (CUP_GRAMMAR_EXPR){94, "TYPE", right_94, 1},
};

CUP_LT_ITEMS item_0 = {
    {"VAR_DEF", 9},
    {"FUNC_STMT", 8},
    {"VAR_STMT", 10},
    {"CODEPART", 3},
    {"S_OR_B", 28},
    {"CODEPART", 6},
    {"SENTENCE", 44},
    {"FUNC_DEF", 7},
    {"CODE", 0},
    {"PARAMETER", 24},
    {"PARAMETERS", 22},
    {"SENTENCES_N", 30},
    {"TYPE", 94},
    {"PARAMETERS_N", 19},
    {"CODEPART", 2},
    {"CODEPART", 5},
    {"ELSE_STC_N", 58},
    {"SENTENCE", 43},
    {"SENTENCES", 33},
    {"PARAMETER", 23},
    {"PARAMETERS", 21},
    {"ARRAY_DEF", 12},
    {"VAR_STMT", 11},
    {"CODEPART", 4},
    {"SENTENCE", 45},
    {"SENTENCES", 32},
    {"CODE", 1},
};
CUP_LT_ITEMS item_1 = {
    {"ASSIGN_EXP", 73},
    {"EXPR", 77},
    {"EXPR", 80},
    {"EXPR_", 82},
    {"EXPR_N", 70},
    {"FUNC_IDENTIFIER", 15},
    {"FOR_EXPRS", 48},
    {"EXPR_", 88},
    {"SENTENCE", 35},
    {"VAR_IDENTIFIERS", 14},
    {"S_OR_B", 28},
    {"ARRAY_VISIT", 76},
    {"ARRAY_VISIT", 75},
    {"COMMA_EXP", 90},
    {"VAR_IDENTIFIER_A", 17},
    {"EXPR", 79},
    {"SENTENCES_N", 30},
    {"ASSIGN_STC", 72},
    {"EXPR_", 87},
    {"VAR_IDENTIFIERS", 13},
    {"PARAMETER_IDF", 26},
    {"SENTENCE", 34},
    {"FUNC_CALL", 93},
    {"ELSE_STC_N", 58},
    {"EXPR_AN", 50},
    {"COMMA_EXP", 89},
    {"COMMA_EXP_ELE", 91},
    {"SENTENCES", 33},
    {"EXPR", 78},
    {"PARAMETER_IDF", 25},
    {"VAR_IDENTIFIER_A", 18},
    {"SENTENCES", 32},
    {"EXPR_AN", 49},
    {"VAR_IDENTIFIER", 16},
};
CUP_LT_ITEMS item_2 = {
    {"BLOCK", 27},
    {"S_OR_B", 29},
};
CUP_LT_ITEMS item_3 = {
    {"ELSE_STC_N", 58},
    {"IF_STC", 56},
    {"SENTENCE", 39},
    {"SENTENCES_N", 30},
    {"SENTENCES", 33},
    {"SENTENCES", 32},
    {"S_OR_B", 28},
};
CUP_LT_ITEMS item_4 = {
    {"COMMA_EXP", 90},
    {"SENTENCE", 34},
    {"EXPR", 77},
    {"EXPR", 80},
    {"ELSE_STC_N", 58},
    {"EXPR_AN", 50},
    {"EXPR_N", 70},
    {"EXPR", 79},
    {"SENTENCES_N", 30},
    {"SENTENCES", 33},
    {"FOR_EXPRS", 48},
    {"EXPR_", 85},
    {"COMMA_EXP", 89},
    {"COMMA_EXP_ELE", 91},
    {"SENTENCES", 32},
    {"S_OR_B", 28},
    {"EXPR", 78},
};
CUP_LT_ITEMS item_5 = {
    {"EXPR", 81},
    {"SENTENCE", 34},
    {"EXPR", 80},
    {"ELSE_STC_N", 58},
    {"EXPR_AN", 50},
    {"EXPR_N", 70},
    {"EXPR", 79},
    {"SENTENCES_N", 30},
    {"SENTENCES", 33},
    {"FOR_EXPRS", 48},
    {"SENTENCES", 32},
    {"S_OR_B", 28},
    {"EXPR", 78},
};
CUP_LT_ITEMS item_6 = {
    {"COMMA_EXP", 90},
    {"SENTENCE", 34},
    {"EXPR", 77},
    {"EXPR", 80},
    {"ELSE_STC_N", 58},
    {"EXPR_AN", 50},
    {"EXPR_N", 70},
    {"EXPR", 79},
    {"SENTENCES_N", 30},
    {"SENTENCES", 33},
    {"FOR_EXPRS", 48},
    {"COMMA_EXP", 89},
    {"COMMA_EXP_ELE", 91},
    {"SENTENCES", 32},
    {"S_OR_B", 28},
    {"EXPR", 78},
    {"EXPR_", 84},
};
CUP_LT_ITEMS item_7 = {
    {"ELSE_STC_N", 58},
    {"FOR_STC", 47},
    {"SENTENCES_N", 30},
    {"SENTENCES", 33},
    {"SENTENCE", 36},
    {"SENTENCES", 32},
    {"S_OR_B", 28},
};
CUP_LT_ITEMS item_8 = {
    {"DO_STC", 53},
    {"ELSE_STC_N", 58},
    {"SENTENCES_N", 30},
    {"SENTENCES", 33},
    {"SENTENCES", 32},
    {"S_OR_B", 28},
    {"SENTENCE", 38},
};
CUP_LT_ITEMS item_9 = {
    {"ELSE_STC_N", 58},
    {"SENTENCE", 41},
    {"SENTENCES_N", 30},
    {"SENTENCES", 33},
    {"BREAK_STC", 54},
    {"SENTENCES", 32},
    {"S_OR_B", 28},
};
CUP_LT_ITEMS item_10 = {
    {"SWITCH_STC", 60},
    {"SENTENCE", 40},
    {"ELSE_STC_N", 58},
    {"SENTENCES_N", 30},
    {"SENTENCES", 33},
    {"SENTENCES", 32},
    {"S_OR_B", 28},
};
CUP_LT_ITEMS item_11 = {
    {"WHILE_STC", 52},
    {"SENTENCE", 37},
    {"ELSE_STC_N", 58},
    {"SENTENCES_N", 30},
    {"SENTENCES", 33},
    {"SENTENCES", 32},
    {"S_OR_B", 28},
};
CUP_LT_ITEMS item_12 = {
    {"COMMA_EXP", 90},
    {"SENTENCE", 34},
    {"EXPR", 77},
    {"EXPR_", 83},
    {"EXPR", 80},
    {"ELSE_STC_N", 58},
    {"EXPR_AN", 50},
    {"EXPR_N", 70},
    {"EXPR", 79},
    {"SENTENCES_N", 30},
    {"SENTENCES", 33},
    {"FOR_EXPRS", 48},
    {"COMMA_EXP", 89},
    {"COMMA_EXP_ELE", 91},
    {"SENTENCES", 32},
    {"S_OR_B", 28},
    {"EXPR", 78},
};
CUP_LT_ITEMS item_13 = {
    {"EXPR", 77},
    {"EXPR", 80},
    {"EXPR_N", 70},
    {"FOR_EXPRS", 48},
    {"SENTENCE", 35},
    {"S_OR_B", 28},
    {"COMMA_EXP", 90},
    {"COMMA_EXP_ELE", 92},
    {"EXPR", 79},
    {"SENTENCES_N", 30},
    {"ASSIGN_STC", 72},
    {"SENTENCE", 34},
    {"ELSE_STC_N", 58},
    {"EXPR_AN", 50},
    {"COMMA_EXP", 89},
    {"COMMA_EXP_ELE", 91},
    {"SENTENCES", 33},
    {"ASSIGN_EXP", 74},
    {"EXPR", 78},
    {"EXPR_", 86},
    {"SENTENCES", 32},
    {"EXPR_AN", 49},
};
CUP_LT_ITEMS item_14 = {
    {"ELSE_STC_N", 58},
    {"CONTINUE_STC", 55},
    {"SENTENCES_N", 30},
    {"SENTENCES", 33},
    {"SENTENCE", 42},
    {"SENTENCES", 32},
    {"S_OR_B", 28},
};
CUP_LT_ITEMS item_15 = {
    {"SENTENCE", 46},
    {"ELSE_STC_N", 58},
    {"RETURN_STC", 69},
    {"SENTENCES_N", 30},
    {"SENTENCES", 33},
    {"SENTENCES", 32},
    {"S_OR_B", 28},
};
CUP_LT_ITEMS item_16 = {
    {"EXPR_AN", 51},
    {"FOR_EXPRS", 48},
    {"EXPR_N", 71},
};
CUP_LT_ITEMS item_17 = {
    {"ELSE_STC_N", 57},
    {"ELSE_STC", 59},
    {"ELSE_STC_N", 58},
};
CUP_LT_ITEMS item_18 = {
    {"CASE_STC", 67},
    {"CS_OR_D", 62},
    {"CASES_N", 63},
    {"ELSE_STC_N", 58},
    {"CS_OR_D", 61},
    {"CASES", 66},
    {"CASES", 65},
};
CUP_LT_ITEMS item_19 = {
    {"DEFAULT_STC", 68},
    {"ELSE_STC_N", 58},
    {"CS_OR_D", 62},
    {"CASES_N", 64},
};
CUP_LT_ITEMS item_20 = {
    {"EXPR_AN", 51},
    {"PARAMETERS_N", 20},
};
CUP_LT_ITEMS item_21 = {
    {"CS_OR_D", 61},
    {"SENTENCES_N", 31},
    {"CASES_N", 64},
    {"ELSE_STC_N", 58},
};

CUP_LT LL1Table = {
    {"<type>", item_0},
    {"<identifier>", item_1},
    {"{", item_2},
    {"<if>", item_3},
    {"<opr_minus>", item_4},
    {"<opr_left>", item_5},
    {"<sizeof>", item_6},
    {"<for>", item_7},
    {"<do>", item_8},
    {"<break>", item_9},
    {"<switch>", item_10},
    {"<while>", item_11},
    {"<number>", item_12},
    {"(", item_13},
    {"<continue>", item_14},
    {"<return>", item_15},
    {";", item_16},
    {"<else>", item_17},
    {"<case>", item_18},
    {"<default>", item_19},
    {")", item_20},
    {"}", item_21},
};
