/*
 * Upstream Altera code dump only included the generated C code
 * and not the source yacc, so we're stuck with this file for now.
 */

/* # line 15 "jamexp.y" */

#include <stdint.h>
#include <math.h>
#include "jamexprt.h"
#include "jamdefs.h"
#include "jamexp.h"
#include "jamsym.h"
#include "jamheap.h"
#include "jamarray.h"
#include "jamutil.h"
#include "jamytab.h"
#ifdef URJ_JAM_YYDEBUG
#include <stdio.h>
#endif // URJ_JAM_YYDEBUG


/* ------------- LEXER DEFINITIONS -----------------------------------------*/
/****************************************************************************/
/*                                                                          */
/*  Operation of GET_FIRST_CH, GET_NEXT_CH, UNGET_CH, and DELETE_CH:        */
/*                                                                          */
/*  Call GET_FIRST_CH to read a character from mdl_lexer_fp and put it into */
/*  urj_jam_ch and urj_jam_token_buffer.                                    */
/*                                                                          */
/*      urj_jam_ch = first char                                             */
/*      urj_jam_token_buffer[0] = first char                                */
/*      urj_jam_token_buffer[1] = '\0';                                     */
/*      urj_jam_token_buffer[2] = ?                                         */
/*      urj_jam_token_buffer[3] = ?                                         */
/*                                                                          */
/*  Call GET_NEXT_CH to read a character from jam_lexer_fp, put it in       */
/*  urj_jam_ch, and append it to urj_jam_token_buffer.                      */
/*                                                                          */
/*      urj_jam_ch = second char                                            */
/*      urj_jam_token_buffer[0] = first char                                */
/*      urj_jam_token_buffer[1] = second char                               */
/*      urj_jam_token_buffer[2] = '\0';                                     */
/*      urj_jam_token_buffer[3] = ?                                         */
/*                                                                          */
/*  Call UNGET_CH remove the last character from the buffer but leave it in */
/*  urj_jam_ch and set a flag.  (The next call to GET_FIRST_CH will use     */
/*  urj_jam_ch as the first char of the token and clear the flag.)          */
/*                                                                          */
/*      urj_jam_ch = second char                                            */
/*      urj_jam_token_buffer[0] = first char                                */
/*      urj_jam_token_buffer[1] = '\0';                                     */
/*      urj_jam_token_buffer[2] = ?                                         */
/*      urj_jam_token_buffer[3] = ?                                         */
/*                                                                          */
/*  Call DELETE_CH to remove the last character from the buffer.  Use this  */
/*  macro to discard the quotes surrounding a string, for example.  Unlike  */
/*  UNGET_CH, the deleted character will not be reused.                     */
/*                                                                          */
/****************************************************************************/

#define MAX_BUFFER_LENGTH   1024
#define END_OF_STRING       -1



/****************************************************************************/
/*                                                                          */
/*  Operation of BEGIN_MACHINE, END_MACHINE, and ACCEPT:                    */
/*                                                                          */
/*  BEGIN_MACHINE and END_MACHINE should be at the beginning the end of an  */
/*  integer function.  Inside the function, define states of the machine    */
/*  with normal C labels, and jump to states with normal C goto statements. */
/*  Use ACCEPT(token) to return an integer value token to the calling       */
/*  routine.                                                                */
/*                                                                          */
/*      int foo (void)                                                      */
/*      {                                                                   */
/*          BEGIN_MACHINE;                                                  */
/*                                                                          */
/*          start:                                                          */
/*              if (whatever) goto next;                                    */
/*              else goto start;                                            */
/*                                                                          */
/*          next:                                                           */
/*              if (done) ACCEPT (a_token_id);                              */
/*              else goto start;                                            */
/*                                                                          */
/*          END_MACHINE;                                                    */
/*      }                                                                   */
/*                                                                          */
/*  Be sure that there is an ACCEPT() or goto at the end of every state.    */
/*  Otherwise, control will "flow" from one state to the next illegally.    */
/*                                                                          */
/****************************************************************************/

#define BEGIN_MACHINE   {int ret

#define ACCEPT(token)   {ret = (token); goto accept;}

#define END_MACHINE     accept: urj_jam_token = ret; \
                        }

struct
{
    char *string;
    int length;
    int token;
} static const jam_keyword_table[] =
{
    {"&&", 2, AND_TOK},
    {"||", 2, OR_TOK},
    {"==", 2, EQUALITY_TOK},
    {"!=", 2, INEQUALITY_TOK},
    {">", 2, GREATER_TOK},
    {"<", 2, LESS_TOK},
    {">=", 2, GREATER_EQ_TOK},
    {"<=", 2, LESS_OR_EQ_TOK},
    {"<<", 2, LEFT_SHIFT_TOK},
    {">>", 2, RIGHT_SHIFT_TOK},
    {"..", 2, DOT_DOT_TOK},
    {"OR", 2, OR_TOK},
    {"AND", 3, AND_TOK},
    {"ABS", 3, ABS_TOK},
    {"INT", 3, INT_TOK},
    {"LOG2", 4, LOG2_TOK},
    {"SQRT", 4, SQRT_TOK},
    {"CEIL", 4, CIEL_TOK},
    {"FLOOR", 5, FLOOR_TOK}
};

char urj_jam_ch = '\0';             /* next character from input file */
int urj_jam_strptr = 0;
int urj_jam_token = 0;
char urj_jam_token_buffer[MAX_BUFFER_LENGTH];
int urj_jam_token_buffer_index;
char urj_jam_parse_string[MAX_BUFFER_LENGTH];
int32_t urj_jam_parse_value = 0;
int urj_jam_expression_type = 0;
JAMS_SYMBOL_RECORD *urj_jam_array_symbol_rec = NULL;

#define YYMAXDEPTH 300          /* This fixes a stack depth problem on  */
                        /* all platforms.                       */

#define YYMAXTLIST 25           /* Max valid next tokens for any state. */
                        /* If there are more, error reporting   */
                        /* will be incomplete.                  */

enum OPERATOR_TYPE
{
    ADD = 0,
    SUB,
    UMINUS,
    MULT,
    DIV,
    MOD,
    NOT,
    AND,
    OR,
    BITWISE_NOT,
    BITWISE_AND,
    BITWISE_OR,
    BITWISE_XOR,
    LEFT_SHIFT,
    RIGHT_SHIFT,
    DOT_DOT,
    EQUALITY,
    INEQUALITY,
    GREATER_THAN,
    LESS_THAN,
    GREATER_OR_EQUAL,
    LESS_OR_EQUAL,
    ABS,
    INT,
    LOG2,
    SQRT,
    CIEL,
    FLOOR,
    ARRAY,
    POUND,
    DOLLAR,
    ARRAY_RANGE,
    ARRAY_ALL
};

typedef enum OPERATOR_TYPE OPERATOR_TYPE;

typedef struct EXP_STACK
{
    OPERATOR_TYPE child_otype;
    JAME_EXPRESSION_TYPE type;
    int32_t val;
    int32_t loper;              /* left and right operands for DIV */
    int32_t roper;              /* we save it for CEIL/FLOOR's use */
} EXPN_STACK;

#define YYSTYPE EXPN_STACK      /* must be a #define for yacc */

YYSTYPE urj_jam_null_expression = { 0, 0, 0, 0, 0 };

JAM_RETURN_TYPE urj_jam_return_code = JAMC_SUCCESS;

JAME_EXPRESSION_TYPE urj_jam_expr_type = JAM_ILLEGAL_EXPR_TYPE;

#define NULL_EXP urj_jam_null_expression    /* .. for 1 operand operators */

#define CALC(operator, lval, rval) urj_jam_exp_eval((operator), (lval), (rval))

/* --- FUNCTION PROTOTYPES -------------------------------------------- */

int urj_jam_yyparse (void);
int urj_jam_yylex (void);

#define AND_TOK 257
#define OR_TOK 258
#define EQUALITY_TOK 259
#define INEQUALITY_TOK 260
#define GREATER_TOK 261
#define LESS_TOK 262
#define GREATER_EQ_TOK 263
#define LESS_OR_EQ_TOK 264
#define LEFT_SHIFT_TOK 265
#define RIGHT_SHIFT_TOK 266
#define DOT_DOT_TOK 267
#define ABS_TOK 268
#define INT_TOK 269
#define LOG2_TOK 270
#define SQRT_TOK 271
#define CIEL_TOK 272
#define FLOOR_TOK 273
#define VALUE_TOK 274
#define IDENTIFIER_TOK 275
#define ARRAY_TOK 276
#define ERROR_TOK 277
#define UNARY_MINUS 278
#define UNARY_PLUS 279
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE urj_jam_yylval, urj_jam_yyval;
#define YYERRCODE 256

/* # line 333 "jamexp.y" */


int32_t urj_jam_convert_bool_to_int (int32_t *data, int32_t msb, int32_t lsb);
EXPN_STACK urj_jam_exp_eval (OPERATOR_TYPE otype, EXPN_STACK op1, EXPN_STACK op2);
void urj_jam_exp_lexer (void);
bool urj_jam_constant_is_ok (const char *string);
bool urj_jam_binary_constant_is_ok (const char *string);
bool urj_jam_hex_constant_is_ok (const char *string);
int32_t urj_jam_atol_bin (char *string);
int32_t urj_jam_atol_hex (char *string);
int urj_jam_constant_value (char *string, int32_t *value);
void urj_jam_yyerror (char *msg);
int urj_jam_yylex (void);
int urj_jam_evaluate_expression (char *expression, int32_t *result,
                             JAME_EXPRESSION_TYPE *result_type);
int urj_jam_yyparse (void);

#define GET_FIRST_CH jam_get_first_ch()
#define GET_NEXT_CH jam_get_next_ch()
#define UNGET_CH jam_unget_ch()
#define CH jam_get_ch()

static inline void jam_get_next_ch(void)
{
    urj_jam_ch = urj_jam_parse_string[urj_jam_strptr++];
    urj_jam_token_buffer [urj_jam_token_buffer_index++] = urj_jam_ch;
    if (urj_jam_token_buffer_index >= MAX_BUFFER_LENGTH)
    {
        --urj_jam_token_buffer_index;
        --urj_jam_strptr;
    }
    urj_jam_token_buffer [urj_jam_token_buffer_index] = '\0';
}

static inline void jam_get_first_ch(void)
{
    urj_jam_token_buffer_index = 0;
    jam_get_next_ch();
}

static inline void jam_unget_ch(void)
{
    urj_jam_strptr--;
    urj_jam_token_buffer[--urj_jam_token_buffer_index] = '\0';
}

static inline char jam_get_ch(void)
{
    return urj_jam_ch;
}

/*
*   Used by INT() function to convert Boolean array data to integer.  "msb"
*   is the index of the most significant bit of the array, and "lsb" is the
*   index of the least significant bit.  Typically msb > lsb, otherwise the
*   bit order will be reversed when converted into integer format.
*/
int32_t
urj_jam_convert_bool_to_int (int32_t *data, int32_t msb, int32_t lsb)
{
    int32_t i, increment = (msb > lsb) ? 1 : -1;
    int32_t mask = 1L, result = 0L;

    msb += increment;
    for (i = lsb; i != msb; i += increment)
    {
        if (data[i >> 5] & (1L << (i & 0x1f)))
            result |= mask;
        mask <<= 1;
    }

    return result;
}


/************************************************************************/
/*                                                                      */

YYSTYPE
urj_jam_exp_eval (OPERATOR_TYPE otype, YYSTYPE op1, YYSTYPE op2)
/*  Evaluate op1 OTYPE op2.  op1, op2 are operands, OTYPE is operator   */
/*                                                                      */
/*  Some sneaky things are done to implement CEIL and FLOOR.            */
/*                                                                      */
/*  We do CEIL of LOG2 by default, and FLOOR of a DIVIDE by default.    */
/*  Since we are lazy and we don't want to generate a parse tree,       */
/*  we use the parser's reduce actions to tell us when to perform       */
/*  an evaluation. But when CEIL and FLOOR are reduced, we know         */
/*  nothing about the expression tree beneath it (it's been reduced!)   */
/*                                                                      */
/*  We keep this information around so we can calculate the CEIL or     */
/*  FLOOR. We save value of the operand(s) or a divide in loper and     */
/*  roper, then when CEIL/FLOOR get reduced, we just look at their      */
/*  values.                                                             */
/*                                                                      */
{
    YYSTYPE rtn;
    int32_t tmp_val;
    JAMS_SYMBOL_RECORD *symbol_rec;
    JAMS_HEAP_RECORD *heap_rec;

    rtn.child_otype = 0;
    rtn.type = JAM_ILLEGAL_EXPR_TYPE;
    rtn.val = 0;
    rtn.loper = 0;
    rtn.roper = 0;

    switch (otype)
    {
    case UMINUS:
        if ((op1.type == JAM_INTEGER_EXPR)
            || (op1.type == JAM_INT_OR_BOOL_EXPR))
        {
            rtn.val = -1 * op1.val;
            rtn.type = JAM_INTEGER_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case ADD:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = op1.val + op2.val;
            rtn.type = JAM_INTEGER_EXPR;

            /* check for overflow */
            if (((op1.val > 0) && (op2.val > 0) && (rtn.val < 0)) ||
                ((op1.val < 0) && (op2.val < 0) && (rtn.val > 0)))
            {
                urj_jam_return_code = JAMC_INTEGER_OVERFLOW;
            }
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case SUB:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = op1.val - op2.val;
            rtn.type = JAM_INTEGER_EXPR;

            /* check for overflow */
            if (((op1.val > 0) && (op2.val < 0) && (rtn.val < 0)) ||
                ((op1.val < 0) && (op2.val > 0) && (rtn.val > 0)))
            {
                urj_jam_return_code = JAMC_INTEGER_OVERFLOW;
            }
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case MULT:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = op1.val * op2.val;
            rtn.type = JAM_INTEGER_EXPR;

            /* check for overflow */
            if ((op1.val != 0) && (op2.val != 0) &&
                (((rtn.val / op1.val) != op2.val) ||
                 ((rtn.val / op2.val) != op1.val)))
            {
                urj_jam_return_code = JAMC_INTEGER_OVERFLOW;
            }
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case DIV:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            if (op2.val != 0)
            {
                rtn.val = op1.val / op2.val;
                rtn.loper = op1.val;
                rtn.roper = op2.val;
                rtn.child_otype = DIV;  /* Save info needed by CEIL */
                rtn.type = JAM_INTEGER_EXPR;
            }
            else
            {
                urj_jam_return_code = JAMC_DIVIDE_BY_ZERO;
            }
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case MOD:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = op1.val % op2.val;
            rtn.type = JAM_INTEGER_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case NOT:
        if ((op1.type == JAM_BOOLEAN_EXPR)
            || (op1.type == JAM_INT_OR_BOOL_EXPR))
        {
            rtn.val = (op1.val == 0) ? 1 : 0;
            rtn.type = JAM_BOOLEAN_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case AND:
        if (((op1.type == JAM_BOOLEAN_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_BOOLEAN_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = (op1.val && op2.val) ? 1 : 0;
            rtn.type = JAM_BOOLEAN_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case OR:
        if (((op1.type == JAM_BOOLEAN_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_BOOLEAN_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = (op1.val || op2.val) ? 1 : 0;
            rtn.type = JAM_BOOLEAN_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case BITWISE_NOT:
        if ((op1.type == JAM_INTEGER_EXPR)
            || (op1.type == JAM_INT_OR_BOOL_EXPR))
        {
            rtn.val = ~(uint32_t) op1.val;
            rtn.type = JAM_INTEGER_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case BITWISE_AND:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = op1.val & op2.val;
            rtn.type = JAM_INTEGER_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case BITWISE_OR:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = op1.val | op2.val;
            rtn.type = JAM_INTEGER_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case BITWISE_XOR:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = op1.val ^ op2.val;
            rtn.type = JAM_INTEGER_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case LEFT_SHIFT:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = op1.val << op2.val;
            rtn.type = JAM_INTEGER_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case RIGHT_SHIFT:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = op1.val >> op2.val;
            rtn.type = JAM_INTEGER_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case EQUALITY:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = (op1.val == op2.val) ? 1 : 0;
            rtn.type = JAM_BOOLEAN_EXPR;
        }
        else if (((op1.type == JAM_BOOLEAN_EXPR)
                  || (op1.type == JAM_INT_OR_BOOL_EXPR))
                 && ((op2.type == JAM_BOOLEAN_EXPR)
                     || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = ((op1.val && op2.val) || ((!op1.val) && (!op2.val)))
                ? 1 : 0;
            rtn.type = JAM_BOOLEAN_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case INEQUALITY:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = (op1.val == op2.val) ? 0 : 1;
            rtn.type = JAM_BOOLEAN_EXPR;
        }
        else if (((op1.type == JAM_BOOLEAN_EXPR)
                  || (op1.type == JAM_INT_OR_BOOL_EXPR))
                 && ((op2.type == JAM_BOOLEAN_EXPR)
                     || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = ((op1.val && op2.val) || ((!op1.val) && (!op2.val)))
                ? 0 : 1;
            rtn.type = JAM_BOOLEAN_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case GREATER_THAN:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = (op1.val > op2.val) ? 1 : 0;
            rtn.type = JAM_BOOLEAN_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case LESS_THAN:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = (op1.val < op2.val) ? 1 : 0;
            rtn.type = JAM_BOOLEAN_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case GREATER_OR_EQUAL:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = (op1.val >= op2.val) ? 1 : 0;
            rtn.type = JAM_BOOLEAN_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case LESS_OR_EQUAL:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            rtn.val = (op1.val <= op2.val) ? 1 : 0;
            rtn.type = JAM_BOOLEAN_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case ABS:
        if ((op1.type == JAM_INTEGER_EXPR) ||
            (op1.type == JAM_INT_OR_BOOL_EXPR))
        {
            rtn.val = (op1.val < 0) ? (0 - op1.val) : op1.val;
            rtn.type = JAM_INTEGER_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case INT:
        rtn.val = op1.val;
        rtn.type = JAM_INTEGER_EXPR;
        break;

    case LOG2:
        if ((op1.type == JAM_INTEGER_EXPR) ||
            (op1.type == JAM_INT_OR_BOOL_EXPR))
        {
            if (op1.val > 0)
            {
                rtn.child_otype = LOG2;
                rtn.type = JAM_INTEGER_EXPR;
                rtn.loper = op1.val;
                tmp_val = op1.val;
                rtn.val = 0;

                while (tmp_val != 1)    /* ret_val = log2(left_val) */
                {
                    tmp_val = tmp_val >> 1;
                    ++rtn.val;
                }

                /* if 2^(return_val) isn't the left_val, then the log */
                /* wasn't a perfect integer, so we increment it */
                if (pow (2, rtn.val) != op1.val)
                {
                    ++rtn.val;  /* Assume ceil of log2 */
                }
            }
            else
            {
                urj_jam_return_code = JAMC_INTEGER_OVERFLOW;
            }
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case SQRT:
        if ((op1.type == JAM_INTEGER_EXPR) ||
            (op1.type == JAM_INT_OR_BOOL_EXPR))
        {
            if (op1.val >= 0)
            {
                rtn.child_otype = SQRT;
                rtn.type = JAM_INTEGER_EXPR;
                rtn.loper = op1.val;
                rtn.val = sqrt (op1.val);
            }
            else
            {
                urj_jam_return_code = JAMC_INTEGER_OVERFLOW;
            }
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case CIEL:
        if ((op1.type == JAM_INTEGER_EXPR)
            || (op1.type == JAM_INT_OR_BOOL_EXPR))
        {
            if (op1.child_otype == DIV)
            {
                /* Below is true if wasn't perfect divide */
                if ((op1.loper * op1.roper) != op1.val)
                {
                    rtn.val = op1.val + 1;      /* add 1 to get CEIL */
                }
                else
                {
                    rtn.val = op1.val;
                }
            }
            else if (op1.child_otype == SQRT)
            {
                /* Below is true if wasn't perfect square-root */
                if ((op1.val * op1.val) < op1.loper)
                {
                    rtn.val = op1.val + 1;      /* add 1 to get CEIL */
                }
                else
                {
                    rtn.val = op1.val;
                }
            }
            else
            {
                rtn.val = op1.val;
            }
            rtn.type = JAM_INTEGER_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case FLOOR:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            if (op1.child_otype == LOG2)
            {
                if (pow (2, op1.val) != op1.loper)
                {
                    rtn.val = op1.val - 1;
                }
                else
                {
                    rtn.val = op1.val;
                }
            }
            else
            {
                rtn.val = op1.val;
            }
            rtn.type = JAM_INTEGER_EXPR;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case ARRAY:
        if ((op1.type == JAM_ARRAY_REFERENCE) &&
            ((op2.type == JAM_INTEGER_EXPR)
             || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            symbol_rec = (JAMS_SYMBOL_RECORD *) op1.val;
            urj_jam_return_code =
                urj_jam_get_array_value (symbol_rec, op2.val, &rtn.val);

            if (urj_jam_return_code == JAMC_SUCCESS)
            {
                switch (symbol_rec->type)
                {
                case JAM_INTEGER_ARRAY_WRITABLE:
                case JAM_INTEGER_ARRAY_INITIALIZED:
                    rtn.type = JAM_INTEGER_EXPR;
                    break;

                case JAM_BOOLEAN_ARRAY_WRITABLE:
                case JAM_BOOLEAN_ARRAY_INITIALIZED:
                    rtn.type = JAM_BOOLEAN_EXPR;
                    break;

                default:
                    urj_jam_return_code = JAMC_INTERNAL_ERROR;
                    break;
                }
            }
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case POUND:
        rtn.val = op1.val;
        rtn.type = JAM_INTEGER_EXPR;
        break;

    case DOLLAR:
        rtn.val = op1.val;
        rtn.type = JAM_INTEGER_EXPR;
        break;

    case ARRAY_RANGE:
        if (((op1.type == JAM_INTEGER_EXPR)
             || (op1.type == JAM_INT_OR_BOOL_EXPR))
            && ((op2.type == JAM_INTEGER_EXPR)
                || (op2.type == JAM_INT_OR_BOOL_EXPR)))
        {
            symbol_rec = urj_jam_array_symbol_rec;

            if ((symbol_rec != NULL) &&
                ((symbol_rec->type == JAM_BOOLEAN_ARRAY_WRITABLE) ||
                 (symbol_rec->type == JAM_BOOLEAN_ARRAY_INITIALIZED)))
            {
                heap_rec = (JAMS_HEAP_RECORD *) symbol_rec->value;

                if (heap_rec != NULL)
                {
                    rtn.val = urj_jam_convert_bool_to_int (heap_rec->data,
                                                       op1.val, op2.val);
                }
                rtn.type = JAM_INTEGER_EXPR;
            }
            else
                urj_jam_return_code = JAMC_TYPE_MISMATCH;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    case ARRAY_ALL:
        if (op1.type == JAM_ARRAY_REFERENCE)
        {
            symbol_rec = (JAMS_SYMBOL_RECORD *) op1.val;

            if ((symbol_rec != NULL) &&
                ((symbol_rec->type == JAM_BOOLEAN_ARRAY_WRITABLE) ||
                 (symbol_rec->type == JAM_BOOLEAN_ARRAY_INITIALIZED)))
            {
                heap_rec = (JAMS_HEAP_RECORD *) symbol_rec->value;

                if (heap_rec != NULL)
                {
                    rtn.val = urj_jam_convert_bool_to_int (heap_rec->data,
                                                       heap_rec->dimension -
                                                       1, 0);
                }
                rtn.type = JAM_INTEGER_EXPR;
            }
            else
                urj_jam_return_code = JAMC_TYPE_MISMATCH;
        }
        else
            urj_jam_return_code = JAMC_TYPE_MISMATCH;
        break;

    default:
        urj_jam_return_code = JAMC_INTERNAL_ERROR;
        break;
    }

    return rtn;
}


/****************************************************************************/
/*                                                                          */

void
urj_jam_exp_lexer (void)
/*                                                                          */
/*  Description:    Lexical analyzer for expressions.                       */
/*                                                                          */
/*                  Results are returned in the global variables            */
/*                  urj_jam_token and urj_jam_token_buffer.                 */
/*                                                                          */
/*  References:     Compilers: Principles, Techniques and Tools by ASU      */
/*                  (the Green Dragon book), section 3.4, Recognition of    */
/*                  tokens.                                                 */
/*                                                                          */
/*  Returns:        Nothing                                                 */
/*                                                                          */
/****************************************************************************/
{
    BEGIN_MACHINE;

  start:
    GET_FIRST_CH;
    if (CH == '\0')
        ACCEPT (END_OF_STRING)  /* Fake an EOF! */
    else if (CH == ' ' || iscntrl (CH))
        goto start;             /* white space */
    else if (isalpha (CH))
        goto identifier;
    else if (isdigit (CH))
        goto number;
    else if (CH == '&')
    {
        GET_NEXT_CH;
        if (CH == '&')
            ACCEPT (AND_TOK)
        else
        {
            UNGET_CH;
            ACCEPT ('&')
        }
    }
    else if (CH == '|')
    {
        GET_NEXT_CH;
        if (CH == '|')
            ACCEPT (OR_TOK)
        else
        {
            UNGET_CH;
            ACCEPT ('|')
        }
    }
    else if (CH == '=')
    {
        GET_NEXT_CH;
        if (CH == '=')
            ACCEPT (EQUALITY_TOK)
        else
        {
            UNGET_CH;
            ACCEPT ('=')
        }
    }
    else if (CH == '!')
    {
        GET_NEXT_CH;
        if (CH == '=')
            ACCEPT (INEQUALITY_TOK)
        else
        {
            UNGET_CH;
            ACCEPT ('!')
        }
    }
    else if (CH == '>')
    {
        GET_NEXT_CH;
        if (CH == '=')
            ACCEPT (GREATER_EQ_TOK)
            else
        if (CH == '>')
            ACCEPT (RIGHT_SHIFT_TOK)
        else
        {
            UNGET_CH;
            ACCEPT (GREATER_TOK)
        }
    }
    else if (CH == '<')
    {
        GET_NEXT_CH;
        if (CH == '=')
            ACCEPT (LESS_OR_EQ_TOK)
            else
        if (CH == '<')
            ACCEPT (LEFT_SHIFT_TOK)
        else
        {
            UNGET_CH;
            ACCEPT (LESS_TOK)
        }
    }
    else if (CH == '.')
    {
        GET_NEXT_CH;
        if (CH == '.')
            ACCEPT (DOT_DOT_TOK)
        else
        {
            UNGET_CH;
            ACCEPT ('.')
        }
    }
    else
        ACCEPT (CH)             /* single-chararcter token */
      number:
        GET_NEXT_CH;
    if (isdigit (CH))
        goto number;
    else if (isalpha (CH) || CH == '_')
        goto identifier;
    else
    {
        UNGET_CH;
        ACCEPT (VALUE_TOK)
    }

  identifier:
    GET_NEXT_CH;
    if (jam_is_name_char(CH))
        goto identifier;
    else
    {
        UNGET_CH;
        ACCEPT (IDENTIFIER_TOK)
    }

    END_MACHINE;
}


/************************************************************************/
/*                                                                      */

bool
urj_jam_constant_is_ok (const char *string)
/*  This routine returns true if the value represented by string is     */
/*  a valid signed decimal number.                                      */
/*                                                                      */
{
    size_t ret = strspn (string, "-0123456789");
    return string[ret] == '\0' ? true : false;
}


/************************************************************************/
/*                                                                      */

bool
urj_jam_binary_constant_is_ok (const char *string)
/*  This routine returns true if the value represented by string is     */
/*  a valid binary number (containing only '0' and '1' characters).     */
/*                                                                      */
{
    size_t ret = strspn (string, "10");
    return string[ret] == '\0' ? true : false;
}


/************************************************************************/
/*                                                                      */

bool
urj_jam_hex_constant_is_ok (const char *string)
/*  This routine returns true if the value represented by string is     */
/*  a valid hexadecimal number.                                         */
/*                                                                      */
{
    size_t ret = strspn (string, "0123456789ABCDEFabcdef");
    return string[ret] == '\0' ? true : false;
}

int32_t
urj_jam_atol_bin (char *string)
{
    int32_t result = 0L;
    int index = 0;

    while ((string[index] == '0') || (string[index] == '1'))
    {
        result = (result << 1) + (string[index] - '0');
        ++index;
    }

    return result;
}

int32_t
urj_jam_atol_hex (char *string)
{
    int32_t result = 0L;

    sscanf (string, "%x", &result);

    return result;
}


/************************************************************************/
/*                                                                      */

int
urj_jam_constant_value (char *string, int32_t *value)
/*                                                                      */
/*      This routine converts a string constant into its binary         */
/*      value.                                                          */
/*                                                                      */
/*      Returns true for success, false if the string could not be      */
/*      converted.                                                      */
/*                                                                      */
{
    int status = false;

    if (urj_jam_expression_type == '#')
    {
        if (urj_jam_binary_constant_is_ok (string))
        {
            *value = urj_jam_atol_bin (string);
            urj_jam_expression_type = 0;
            status = true;
        }
    }
    else if (urj_jam_expression_type == '$')
    {
        if (urj_jam_hex_constant_is_ok (string))
        {
            *value = urj_jam_atol_hex (string);
            urj_jam_expression_type = 0;
            status = true;
        }
    }
    else if (urj_jam_constant_is_ok (string))
    {
        if (string[0] == '-')
        {
            *value = 0 - atol (&string[1]);
        }
        else
        {
            *value = atol (string);
        }
        status = true;
    }

    return status;
}


/************************************************************************/
/*                                                                      */

void
urj_jam_yyerror (char *msg)
/*                                                                      */
/*  WARNING: When compiling for YACC 5.0 using err_skel.c,              */
/*           this function needs to be modified to be:                  */
/*                                                                      */
/*           urj_jam_yyerror(char *ms1, char *msg2)                     */
/*                                                                      */
/*  urj_jam_yyerror() handles syntax error messages from the parser.    */
/*  Since we don't care about anything else but reporting the error,    */
/*  just flag the error in urj_jam_return_code.                         */
/*                                                                      */
{
    msg = msg;                  /* Avoid compiler warning about msg unused */

    if (urj_jam_return_code == JAMC_SUCCESS)
        urj_jam_return_code = JAMC_SYNTAX_ERROR;
}


/************************************************************************/
/*                                                                      */

int
urj_jam_yylex (void)
/*                                                                      */
/*  This is the lexer function called by urj_jam_yyparse(). It calls            */
/*  urj_jam_exp_lexer() to run as the DFA to return a token in urj_jam_token    */
/*                                                                      */
{
    JAMS_SYMBOL_RECORD *symbol_rec = NULL;
    int32_t val = 0L;
    JAME_EXPRESSION_TYPE type = JAM_ILLEGAL_EXPR_TYPE;
    int token_length;
    int i;

    urj_jam_exp_lexer ();

    token_length = strlen (urj_jam_token_buffer);

    if (token_length > 1)
    {
        for (i = 0; i < ARRAY_SIZE(jam_keyword_table); i++)
        {
            if (token_length == jam_keyword_table[i].length)
            {
                if (!strcmp
                    (urj_jam_token_buffer, jam_keyword_table[i].string))
                {
                    urj_jam_token = jam_keyword_table[i].token;
                }
            }
        }
    }

    if (urj_jam_token == VALUE_TOK)
    {
        if (urj_jam_constant_value (urj_jam_token_buffer, &val))
        {
            /* literal 0 and 1 may be interpreted as Integer or Boolean */
            if ((val == 0) || (val == 1))
            {
                type = JAM_INT_OR_BOOL_EXPR;
            }
            else
            {
                type = JAM_INTEGER_EXPR;
            }
        }
        else
        {
            urj_jam_return_code = JAMC_SYNTAX_ERROR;
        }
    }
    else if (urj_jam_token == IDENTIFIER_TOK)
    {
        urj_jam_return_code =
            urj_jam_get_symbol_record (urj_jam_token_buffer, &symbol_rec);

        if (urj_jam_return_code == JAMC_SUCCESS)
        {
            switch (symbol_rec->type)
            {
            case JAM_INTEGER_SYMBOL:
                /* Success, swap token to be a VALUE */
                urj_jam_token = VALUE_TOK;
                val = symbol_rec->value;
                type = JAM_INTEGER_EXPR;
                break;

            case JAM_BOOLEAN_SYMBOL:
                /* Success, swap token to be a VALUE */
                urj_jam_token = VALUE_TOK;
                val = symbol_rec->value ? 1 : 0;
                type = JAM_BOOLEAN_EXPR;
                break;

            case JAM_INTEGER_ARRAY_WRITABLE:
            case JAM_BOOLEAN_ARRAY_WRITABLE:
            case JAM_INTEGER_ARRAY_INITIALIZED:
            case JAM_BOOLEAN_ARRAY_INITIALIZED:
                /* Success, swap token to be an ARRAY_TOK, */
                /* save pointer to symbol record in value field */
                urj_jam_token = ARRAY_TOK;
                val = (int32_t) symbol_rec;
                type = JAM_ARRAY_REFERENCE;
                urj_jam_array_symbol_rec = symbol_rec;
                break;

            default:
                urj_jam_return_code = JAMC_SYNTAX_ERROR;
                break;
            }
        }
    }
    else if (urj_jam_token == '#')
    {
        urj_jam_expression_type = '#';
    }
    else if (urj_jam_token == '$')
    {
        urj_jam_expression_type = '$';
    }

    urj_jam_yylval.val = val;
    urj_jam_yylval.type = type;
    urj_jam_yylval.child_otype = 0;
    urj_jam_yylval.loper = 0;
    urj_jam_yylval.roper = 0;

    return urj_jam_token;
}


/************************************************************************/
/*                                                                      */

JAM_RETURN_TYPE urj_jam_evaluate_expression
    (char *expression, int32_t *result, JAME_EXPRESSION_TYPE *result_type)
/*                                                                      */
/*  THIS IS THE ENTRY POINT INTO THE EXPRESSION EVALUATOR.              */
/*                                                                      */
/*  s = a string representing the expression to be evaluated.           */
/*      (e.g. "2+2+PARAMETER")                                          */
/*                                                                      */
/*  status = for returning true if evaluation was successful.           */
/*           false if not.                                              */
/*                                                                      */
/*  This routine sets up the global variables and then calls            */
/*  urj_jam_yyparse() to do the parsing. The reduce actions of the      */
/*  parser evaluate the expression.                                     */
/*                                                                      */
/*  RETURNS: Value of the expression if success. 0 if FAIL.             */
/*                                                                      */
/*  Note: One should not rely on the return val to det.  success/fail   */
/*        since it is possible for, say, "2-2" to be success and        */
/*        return 0.                                                     */
/*                                                                      */
{
    strcpy (urj_jam_parse_string, expression);
    urj_jam_strptr = 0;
    urj_jam_token_buffer_index = 0;
    urj_jam_return_code = JAMC_SUCCESS;

    urj_jam_yyparse ();

    if (urj_jam_return_code == JAMC_SUCCESS)
    {
        if (result != 0)
            *result = urj_jam_parse_value;
        if (result_type != 0)
            *result_type = urj_jam_expr_type;
    }

    return urj_jam_return_code;
}

static const int jam_yyexca[] = {
    -1, 1,
    0, -1,
    -2, 0,
    0,
};

#define YYNPROD 37
#define YYLAST 626

static const int jam_yyact[] = {
    7, 67, 68, 79, 45, 76, 66, 4,
    20, 7, 5, 1, 6, 18, 44, 43,
    4, 20, 19, 5, 0, 6, 18, 16,
    42, 17, 41, 19, 40, 39, 0, 0,
    0, 20, 21, 0, 0, 0, 18, 16,
    0, 17, 0, 19, 20, 21, 0, 0,
    0, 18, 16, 0, 17, 0, 19, 0,
    20, 21, 0, 0, 86, 18, 16, 0,
    17, 0, 19, 20, 21, 0, 0, 83,
    18, 16, 0, 17, 0, 19, 20, 21,
    0, 0, 82, 18, 16, 0, 17, 0,
    19, 0, 23, 0, 0, 8, 0, 0,
    0, 0, 20, 0, 89, 23, 8, 18,
    16, 0, 17, 0, 19, 0, 0, 0,
    84, 23, 0, 0, 0, 20, 21, 0,
    22, 81, 18, 16, 23, 17, 0, 19,
    20, 21, 0, 22, 80, 18, 16, 23,
    17, 0, 19, 0, 20, 21, 0, 22,
    75, 18, 16, 0, 17, 0, 19, 0,
    0, 0, 22, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 22, 0, 0,
    0, 0, 0, 0, 0, 0, 23, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 23, 0, 0, 0, 0, 20, 21,
    0, 0, 0, 18, 16, 23, 17, 0,
    19, 0, 0, 0, 22, 0, 0, 0,
    20, 0, 0, 0, 0, 18, 16, 22,
    17, 0, 19, 0, 20, 0, 0, 0,
    0, 18, 16, 22, 17, 0, 19, 0,
    0, 0, 0, 9, 10, 11, 12, 13,
    14, 3, 69, 15, 9, 10, 11, 12,
    13, 14, 3, 0, 15, 24, 25, 28,
    29, 30, 31, 32, 33, 26, 27, 87,
    24, 25, 28, 29, 30, 31, 32, 33,
    26, 27, 0, 0, 24, 25, 28, 29,
    30, 31, 32, 33, 26, 27, 0, 24,
    25, 28, 29, 30, 31, 32, 33, 26,
    27, 0, 24, 25, 28, 29, 30, 31,
    32, 33, 26, 27, 0, 0, 0, 0,
    0, 20, 21, 0, 0, 64, 18, 16,
    0, 17, 0, 19, 20, 21, 26, 27,
    0, 18, 16, 0, 17, 0, 19, 0,
    0, 24, 25, 28, 29, 30, 31, 32,
    33, 26, 27, 0, 24, 25, 28, 29,
    30, 31, 32, 33, 26, 27, 0, 0,
    24, 25, 28, 29, 30, 31, 32, 33,
    26, 27, 23, 0, 20, 21, 0, 0,
    0, 18, 16, 0, 17, 23, 19, 20,
    21, 0, 0, 0, 18, 16, 0, 17,
    0, 19, 0, 0, 20, 21, 0, 0,
    22, 18, 16, 0, 17, 0, 19, 0,
    0, 0, 0, 22, 28, 29, 30, 31,
    32, 33, 26, 27, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 23, 28, 29,
    30, 31, 32, 33, 26, 27, 0, 0,
    23, 0, 0, 0, 30, 31, 32, 33,
    26, 27, 0, 0, 0, 23, 0, 0,
    0, 0, 0, 22, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 22, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 0,
    0, 0, 0, 34, 35, 36, 37, 38,
    0, 0, 0, 0, 0, 0, 0, 46,
    47, 48, 49, 50, 51, 52, 53, 54,
    55, 56, 57, 58, 59, 60, 61, 62,
    63, 0, 0, 0, 0, 0, 65, 0,
    70, 71, 72, 73, 74, 24, 25, 28,
    29, 30, 31, 32, 33, 26, 27, 0,
    24, 25, 28, 29, 30, 31, 32, 33,
    26, 27, 77, 78, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 85, 0,
    0, 0, 0, 0, 0, 0, 88, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    24, 0, 28, 29, 30, 31, 32, 33,
    26, 27, 0, 0, 0, 28, 29, 30,
    31, 32, 33, 26, 27, 0, 0, 0,
    0, 0, 28, 29, 30, 31, 32, 33,
    26, 27,
};

static const int jam_yypact[] = {
    -24, -1000, 287, -1000, -24, -24, -24, -24,
    -24, -11, -12, -14, -16, -25, -26, -87,
    -24, -24, -24, -24, -24, -24, -24, -24,
    -24, -24, -24, -24, -24, -24, -24, -24,
    -24, -24, 276, -1000, -1000, -1000, -1000, -24,
    -34, -24, -24, -24, -24, -24, -29, -29,
    -1000, -1000, -1000, 171, 359, 153, 346, 335,
    -20, -20, 183, 183, 61, 61, 61, 61,
    -1000, 103, -36, -24, -24, -88, 91, 80,
    41, 30, 19, -1000, -1000, 287, 287, -33,
    -1000, -1000, -1000, -1000, -1000, -4, -1000, -24,
    7, -1000,
};

static const int jam_yypgo[] = {
    0, 11, 486, 6,
};

static const int jam_yyr1[] = {
    0, 1, 3, 3, 3, 3, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2,
};

static const int jam_yyr2[] = {
    0, 1, 2, 2, 6, 3, 1, 3,
    2, 2, 2, 2, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 4, 4,
    4, 4, 4, 4, 4,
};

static const int jam_yychk[] = {
    -1000, -1, -2, 274, 40, 43, 45, 33,
    126, 268, 269, 270, 271, 272, 273, 276,
    43, 45, 42, 47, 37, 38, 124, 94,
    257, 258, 265, 266, 259, 260, 261, 262,
    263, 264, -2, -2, -2, -2, -2, 40,
    40, 40, 40, 40, 40, 91, -2, -2,
    -2, -2, -2, -2, -2, -2, -2, -2,
    -2, -2, -2, -2, -2, -2, -2, -2,
    41, -2, -3, 35, 36, 276, -2, -2,
    -2, -2, -2, 41, 41, -2, -2, 91,
    41, 41, 41, 41, 93, -2, 93, 267,
    -2, 93,
};

static const int jam_yydef[] = {
    0, -2, 1, 6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 8, 9, 10, 11, 0,
    0, 0, 0, 0, 0, 0, 12, 13,
    14, 15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29,
    7, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 30, 31, 2, 3, 0,
    32, 33, 34, 35, 36, 0, 5, 0,
    0, 4,
};

/****************************************************************************/
/*                                                                          */
/*  Module:         jamycskl.c                                              */
/*                                                                          */
/*                  Copyright (C) Altera Corporation 1997                   */
/*                                                                          */
/*  Description:    LALR parser driver skeleton file -- used by YACC        */
/*                                                                          */
/****************************************************************************/


#ifndef INITIALIZE
#define INITIALIZE
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 200          /* default stack depth */
#endif

#ifndef jam_yyerrok
#define jam_yyerrok ((int) 0)
#endif

#define YYFLAG -1000
#define YYERROR goto jam_yyerrlab
#define YYACCEPT return(0)
#define YYABORT return(1)

static YYSTYPE jam_yyv[YYMAXDEPTH];
static int token = -1;                 /* input token */
static int errct = 0;                  /* error count */
static int errfl = 0;                  /* error flag */

int
urj_jam_yyparse (void)
{
    int jam_yys[YYMAXDEPTH];
    int jam_yyj, jam_yym;
    YYSTYPE *jam_yypvt;
    int jam_yystate, *jam_yyps, jam_yyn;

    const int *jam_yyxi;

    YYSTYPE *jam_yypv;

    jam_yystate = 0;
    token = -1;
    errct = 0;
    errfl = 0;
    jam_yyps = &jam_yys[-1];
    jam_yypv = &jam_yyv[-1];


  jam_yystack:                 /* put a state and value onto the stack */

    if (++jam_yyps > &jam_yys[YYMAXDEPTH])
    {
        urj_jam_yyerror ("yacc stack overflow");
        return 1;
    }
    *jam_yyps = jam_yystate;
    ++jam_yypv;
    *jam_yypv = urj_jam_yyval;

  jam_yynewstate:

    jam_yyn = jam_yypact[jam_yystate];

    if (jam_yyn <= YYFLAG)
        goto jam_yydefault;     /* simple state */

    if (token < 0)
        if ((token = urj_jam_yylex ()) < 0)
            token = 0;
    if ((jam_yyn += token) < 0 || jam_yyn >= YYLAST)
        goto jam_yydefault;

    if (jam_yychk[jam_yyn = jam_yyact[jam_yyn]] == token)
    {                           /* valid shift */
#ifdef URJ_JAM_YYDEBUG
        if (token <= 0xff)
        {
            printf("# .. shift '%c' -> state %d\n", token, jam_yyn);
        }
        else
        {
            // symbols are NOT jam_keyword_table[*].token!
            printf("# .. shift %d -> state %d\n", token, jam_yyn);
        }
#endif // URJ_JAM_YYDEBUG
        token = -1;
        urj_jam_yyval = urj_jam_yylval;
        jam_yystate = jam_yyn;
        if (errfl > 0)
            --errfl;
        goto jam_yystack;
    }

  jam_yydefault:

    if ((jam_yyn = jam_yydef[jam_yystate]) == -2)
    {
        if (token < 0)
            if ((token = urj_jam_yylex ()) < 0)
                token = 0;
        /* look through exception table */

        for (jam_yyxi = jam_yyexca; (*jam_yyxi != (-1)) || (jam_yyxi[1] != jam_yystate); jam_yyxi += 2);        /* VOID */

        while (*(jam_yyxi += 2) >= 0)
        {
            if (*jam_yyxi == token)
                break;
        }
        if ((jam_yyn = jam_yyxi[1]) < 0)
            return 0;         /* accept */
    }

    if (jam_yyn == 0)
    {                           /* error */

        switch (errfl)
        {
        case 0:                /* brand new error */
            urj_jam_yyerror ("syntax error");
            /* jam_yyerrlab: */
            ++errct;

        case 1:
        case 2:                /* incompletely recovered error ... try again */
            errfl = 3;

            /* find a state where "error" is a legal shift action */

            while (jam_yyps >= jam_yys)
            {
                jam_yyn = jam_yypact[*jam_yyps] + YYERRCODE;
                if (jam_yyn >= 0 && jam_yyn < YYLAST
                    && jam_yychk[jam_yyact[jam_yyn]] == YYERRCODE)
                {
                    jam_yystate = jam_yyact[jam_yyn];   /* simulate a shift of "error" */
                    goto jam_yystack;
                }
                jam_yyn = jam_yypact[*jam_yyps];
                /* the current jam_yyps has no shift onn "error", pop stack */
                --jam_yyps;
                --jam_yypv;
            }

            /* there is no state on the stack with an error shift ... abort */

          jam_yyabort:
            return 1;
        case 3:                /* no shift yet; clobber input char */

            if (token == 0)
                goto jam_yyabort;       /* don't discard EOF, quit */
            token = -1;
            goto jam_yynewstate;        /* try again in the same state */
        }

    }

    /* reduction by production jam_yyn */

    jam_yyps -= jam_yyr2[jam_yyn];
    jam_yypvt = jam_yypv;
    jam_yypv -= jam_yyr2[jam_yyn];
    urj_jam_yyval = jam_yypv[1];
    jam_yym = jam_yyn;
    /* consult goto table to find next state */
    jam_yyn = jam_yyr1[jam_yyn];
    jam_yyj = jam_yypgo[jam_yyn] + *jam_yyps + 1;
    if (jam_yyj >= YYLAST
        || jam_yychk[jam_yystate = jam_yyact[jam_yyj]] != -jam_yyn)
        jam_yystate = jam_yyact[jam_yypgo[jam_yyn]];
#ifdef URJ_JAM_YYDEBUG
    printf("# .. reduce P%d:", jam_yym);
    for (int ri = -jam_yyr2[jam_yym] + 1; ri <= 0; ++ ri)
    {
        printf(" (t=%d v=%d)", jam_yypvt[ri].type, jam_yypvt[ri].val);
    }
    printf(" -> sym %d, state %d\n", jam_yyn, jam_yystate);
#endif // URJ_JAM_YYDEBUG
    switch (jam_yym)
    {

    case 1:
/* # line 288 "jamexp.y" */
        {
            urj_jam_parse_value = jam_yypvt[-0].val;
            urj_jam_expr_type = jam_yypvt[-0].type;
        }
        break;
    case 2:
/* # line 292 "jamexp.y" */
        {
            urj_jam_yyval = CALC (POUND, jam_yypvt[-0], NULL_EXP);
        }
        break;
    case 3:
/* # line 293 "jamexp.y" */
        {
            urj_jam_yyval = CALC (DOLLAR, jam_yypvt[-0], NULL_EXP);
        }
        break;
    case 4:
/* # line 295 "jamexp.y" */
        {
            urj_jam_yyval = CALC (ARRAY_RANGE, jam_yypvt[-3], jam_yypvt[-1]);
        }
        break;
    case 5:
/* # line 296 "jamexp.y" */
        {
            urj_jam_yyval = CALC (ARRAY_ALL, jam_yypvt[-2], NULL_EXP);
        }
        break;
    case 7:
/* # line 301 "jamexp.y" */
        {
            urj_jam_yyval = jam_yypvt[-1];
        }
        break;
    case 8:
/* # line 302 "jamexp.y" */
        {
            urj_jam_yyval = jam_yypvt[-0];
        }
        break;
    case 9:
/* # line 303 "jamexp.y" */
        {
            urj_jam_yyval = CALC (UMINUS, jam_yypvt[-0], NULL_EXP);
        }
        break;
    case 10:
/* # line 304 "jamexp.y" */
        {
            urj_jam_yyval = CALC (NOT, jam_yypvt[-0], NULL_EXP);
        }
        break;
    case 11:
/* # line 305 "jamexp.y" */
        {
            urj_jam_yyval = CALC (BITWISE_NOT, jam_yypvt[-0], NULL_EXP);
        }
        break;
    case 12:
/* # line 306 "jamexp.y" */
        {
            urj_jam_yyval = CALC (ADD, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 13:
/* # line 307 "jamexp.y" */
        {
            urj_jam_yyval = CALC (SUB, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 14:
/* # line 308 "jamexp.y" */
        {
            urj_jam_yyval = CALC (MULT, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 15:
/* # line 309 "jamexp.y" */
        {
            urj_jam_yyval = CALC (DIV, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 16:
/* # line 310 "jamexp.y" */
        {
            urj_jam_yyval = CALC (MOD, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 17:
/* # line 311 "jamexp.y" */
        {
            urj_jam_yyval = CALC (BITWISE_AND, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 18:
/* # line 312 "jamexp.y" */
        {
            urj_jam_yyval = CALC (BITWISE_OR, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 19:
/* # line 313 "jamexp.y" */
        {
            urj_jam_yyval = CALC (BITWISE_XOR, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 20:
/* # line 314 "jamexp.y" */
        {
            urj_jam_yyval = CALC (AND, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 21:
/* # line 315 "jamexp.y" */
        {
            urj_jam_yyval = CALC (OR, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 22:
/* # line 316 "jamexp.y" */
        {
            urj_jam_yyval = CALC (LEFT_SHIFT, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 23:
/* # line 317 "jamexp.y" */
        {
            urj_jam_yyval = CALC (RIGHT_SHIFT, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 24:
/* # line 318 "jamexp.y" */
        {
            urj_jam_yyval = CALC (EQUALITY, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 25:
/* # line 319 "jamexp.y" */
        {
            urj_jam_yyval = CALC (INEQUALITY, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 26:
/* # line 320 "jamexp.y" */
        {
            urj_jam_yyval = CALC (GREATER_THAN, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 27:
/* # line 321 "jamexp.y" */
        {
            urj_jam_yyval = CALC (LESS_THAN, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 28:
/* # line 322 "jamexp.y" */
        {
            urj_jam_yyval = CALC (GREATER_OR_EQUAL, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 29:
/* # line 323 "jamexp.y" */
        {
            urj_jam_yyval = CALC (LESS_OR_EQUAL, jam_yypvt[-2], jam_yypvt[-0]);
        }
        break;
    case 30:
/* # line 324 "jamexp.y" */
        {
            urj_jam_yyval = CALC (ABS, jam_yypvt[-1], NULL_EXP);
        }
        break;
    case 31:
/* # line 325 "jamexp.y" */
        {
            urj_jam_yyval = CALC (INT, jam_yypvt[-1], NULL_EXP);
        }
        break;
    case 32:
/* # line 326 "jamexp.y" */
        {
            urj_jam_yyval = CALC (LOG2, jam_yypvt[-1], NULL_EXP);
        }
        break;
    case 33:
/* # line 327 "jamexp.y" */
        {
            urj_jam_yyval = CALC (SQRT, jam_yypvt[-1], NULL_EXP);
        }
        break;
    case 34:
/* # line 328 "jamexp.y" */
        {
            urj_jam_yyval = CALC (CIEL, jam_yypvt[-1], NULL_EXP);
        }
        break;
    case 35:
/* # line 329 "jamexp.y" */
        {
            urj_jam_yyval = CALC (FLOOR, jam_yypvt[-1], NULL_EXP);
        }
        break;
    case 36:
/* # line 330 "jamexp.y" */
        {
            urj_jam_yyval = CALC (ARRAY, jam_yypvt[-3], jam_yypvt[-1]);
        }
        break;
    }
    goto jam_yystack;           /* stack new state and value */
}
