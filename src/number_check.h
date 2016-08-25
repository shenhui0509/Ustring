#ifndef USTR_NUMBER_CHECK_H_
#define USTR_NUMBER_CHECK_H_

/* dependencies : boost/mpl
 * Why this ? compile time dispatch rather than run time state machine
 */
#include <string>
#include "static_fsm.h"

/* using a state machine to check whether a string is a valid number
 * eg. "0.1332e10" is valid
 *     "+0.12e-100" is valid
 *     "00000012"  is valid
 *     "abc"      is invalid
 */
struct tag{};

class number_check :
  public state_machine<number_check>
{
public:
 bool check(const std::string &str);
private:
 typedef tag  Sign; // '+' and '-'
 typedef tag  Num;  // '0' - '9'
 typedef tag  Dot;  // '.'
 typedef tag  Exp;  // 'e'
 typedef tag  Invalid; // other input, which cannot be presented in a number
 
 enum states {
  SInit = 0, SInitNum, SSign, SDot, SExp, SDotNum, SExpNum, SInitDot, SExpSign
 };
 
 static const  states initial_state = SInit;
 
 void on_sign(Sign const&){}
 void on_num(Num const&){}
 void on_dot(Dot const&){}
 void on_exp(Exp const&){}
 friend class state_machine<number_check>;
 typedef number_check n;
 
 struct transition_table : mpl::vector::vector17<
 //   CurrentState       Event       NextState      Action
 // +----------------+------------+--------------+--------------------+
 row<    SInit       ,    Num     ,   SInitNum   ,  &n::on_num    >,
 row<    SInit       ,    Sign    ,   SSign      ,  &n::on_sign   >,
 row<    SInit       ,    Dot     ,   SInitDot   ,  &n::on_dot    >,
 row<    SInitNum    ,    Num     ,   SInitNum   ,  &n::on_num    >,
 row<    SInitNum    ,    Dot     ,   SDot       ,  &n::on_dot    >,
 row<    SInitNum    ,    Exp     ,   SExp       ,  &n::on_exp    >,
 row<    SSign       ,    Dot     ,   SInitDot   ,  &n::on_dot    >,
 row<    SSign       ,    Num     ,   SInitNum   ,  &n::on_num    >,
 row<    SDot        ,    Exp     ,   SExp       ,  &n::on_exp    >,
 row <   SDot        ,    Num     ,   SDotNum    ,  &n::on_exp    >,
 row <   SExp        ,    Num     ,   SExpNum    ,  &n::on_num    >,
 row <   SExp        ,    Sign    ,   SExpSign   ,  &n::on_sign   >,
 row <   SDotNum     ,    Num     ,   SDotNum    ,  &n::on_num    >,
 row <   SDotNum     ,    Exp     ,   SExp       ,  &n::on_exp    >,
 row <   SExpNum     ,    Num     ,   SExpNum    ,  &n::on_num    >,
 row <   SInitDot    ,    Num     ,   SDotNum    ,  &n::on_num    >,
 row <   SExpSign    ,    Num     ,   SExpNum    ,  &n::on_num    >
 >{};
 
};
#endif
