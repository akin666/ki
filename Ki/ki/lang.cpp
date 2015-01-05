/*
 * kiparser
 *
 *  Created on: 21.10.2014
 *      Author: akin
 */

#include "lang"
#include <string>

namespace ki
{
    TypeMap str2type {
        {
            1,
            {
                { "=" , KI_ASSIGN },
                { ":" , KI_ASSIGN },
//                { "," , KI_SEPARATOR },
                { ";" , KI_END },
                
                { "'" , KI_QUOTATION },
                { "\"" ,KI_QUOTATION },
                
                { "+" , KI_ADD },
                { "-" , KI_SUBTRACT },
                { "/" , KI_DIVIDE },
                { "*" , KI_MULTIPLY },
                { "%" , KI_MODULO },
                
                { "{" , KI_BLOCK_BEGIN },
                { "}" , KI_BLOCK_END },
                { "(" , KI_PARAM_BEGIN },
                { ")" , KI_PARAM_END },
                
                { "[" , KI_ENTITY_BEGIN },
                { "]" , KI_ENTITY_END },
                
                { "!" , KI_NOT },
                { "|" , KI_BOR },
                { "&" , KI_BAND },
                { "\\" ,KI_SPECIAL },
            }
        },
        {
            2,
            {
                { "==" , KI_EQUALS },
                { "!=" , KI_NOT_EQUALS },
                
                { "||" , KI_OR },
                { "&&" , KI_AND },
                
                { "if" , KI_IF },
            }
        },
        {
            3,
            {
                { "var" , KI_VAR },
                { "new" , KI_NEW },
                { "for" , KI_FOR },
                { "NOP" , KI_NOP },
            }
        },
        {
            4,
            {
                { "bool" , KI_BOOL },
                { "null" , KI_NULL },
                { "type" , KI_TYPE },
                { "else" , KI_ELSE },
                { "elif" , KI_ELIF },
            }
        },
        {
            5,
            {
                { "class" , KI_CLASS },
                { "while" , KI_WHILE },
                { "break" , KI_BREAK },
            }
        },
        {
            6,
            {
                { "string" , KI_STRING },
                { "number" , KI_NUMBER },
                { "system" , KI_SYSTEM },
                { "return" , KI_RETURN },
            }
        },
        {
            7,
            {
                { "library" , KI_LIBRARY },
                { "release" , KI_RELEASE },
            }
        },
        {
            8,
            {
                { "continue" , KI_CONTINUE },
                { "function" , KI_FUNCTION },
            }
        },
        {
            9,
            {
                { "namespace" , KI_NAMESPACE },
            }
        }
    };
    
    std::unordered_map<int, std::string> types2str {
        // 0
        { KI_FRAGMENT ,     "fragment" },
        { KI_UNKNOWN ,      "unknown" },
        { KI_WHITESPACE ,   "whitespace" },
        { KI_NOP ,          "NOP" },
        
        // 1
        { KI_ASSIGN ,       "=" },
//        { KI_SEPARATOR ,    "," },
        { KI_END ,          ";" },
        { KI_QUOTATION ,    "\""},
        { KI_ADD ,          "+" },
        { KI_SUBTRACT ,     "-" },
        { KI_DIVIDE  ,      "/" },
        { KI_MULTIPLY ,     "*" },
        { KI_MODULO ,       "%" },
        { KI_BLOCK_BEGIN ,  "{" },
        { KI_BLOCK_END ,    "}" },
        { KI_PARAM_BEGIN ,  "(" },
        { KI_PARAM_END ,    ")" },
        { KI_ENTITY_BEGIN , "[" },
        { KI_ENTITY_END ,   "]" },
        { KI_NOT ,          "!" },
        { KI_BOR ,          "|" },
        { KI_BAND ,         "&" },
        { KI_SPECIAL ,      "\\"},
        
        // 2
        { KI_EQUALS ,       "==" },
        { KI_NOT_EQUALS ,   "!=" },
        { KI_OR ,           "||" },
        { KI_AND ,          "&&" },
        { KI_IF ,           "if" },
        
        // 3
        { KI_VAR ,          "var" },
        { KI_NEW ,          "new" },
        { KI_FOR ,          "for" },
        { KI_NOP ,          "NOP" },
        
        // 4
        { KI_BOOL ,         "bool" },
        { KI_NULL ,         "null" },
        { KI_TYPE ,         "type" },
        { KI_ELSE ,         "else" },
        { KI_ELIF ,         "elif" },
        
        // 5
        { KI_CLASS ,        "class" },
        { KI_WHILE ,        "while" },
        { KI_BREAK ,        "break" },
        
        // 6
        { KI_STRING ,       "string" },
        { KI_NUMBER ,       "number" },
        { KI_SYSTEM ,       "system" },
        { KI_RETURN ,       "return" },
        
        // 7
        { KI_LIBRARY ,      "library" },
        { KI_RELEASE ,      "release" },
        
        // 8
        { KI_CONTINUE ,     "continue" },
        { KI_FUNCTION ,     "function" },
        
        // 9
        { KI_NAMESPACE ,    "namespace" },
    };
    
    Type recognize( const char *cstr , size_t length )
    {
        return recognize(std::string(cstr , length));
    }
    
    Type recognize( const std::string& str )
    {
        const auto length = str.size();
        auto iter = str2type.find(length);
        
        if( iter != str2type.end() )
        {
            auto sc = iter->second;
            auto iter2 = sc.find(str);
            if( iter2 != sc.end() )
            {
                return iter2->second;
            }
        }
        
        return KI_FRAGMENT;
    }

    std::string toString( Type type )
    {
        auto iter = types2str.find( type );
        if( iter == types2str.end() )
        {
            return "";
        }
        return iter->second;
    }
}