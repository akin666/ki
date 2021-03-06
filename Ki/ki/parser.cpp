/*
 * ki
 *
 *  Created on: 19.12.2014
 *      Author: akin
 */

#include "parser"
#include <iostream>

namespace ki
{
	namespace parser
    {
        Type recognize( const std::string& key , const TypeMap& types )
        {
            auto type = ::ki::recognize( key );
            
            if( type != KI_FRAGMENT )
            {
                return type;
            }
            
            // not found, lets search from context specific
            const auto length = key.size();
            auto iter = types.find(length);
            
            if( iter != types.end() )
            {
                auto sc = iter->second;
                auto iter2 = sc.find(key);
                if( iter2 != sc.end() )
                {
                    return iter2->second;
                }
            }
            
            return KI_FRAGMENT;
        }
        
        Type recognize( const char *carray , size_t length , const TypeMap& types  )
        {
            return recognize(std::string(carray , length) , types );
        }
                
        size_t seekString( const char *carray , size_t max , size_t at )
        {
            auto quotation = carray[at];
            
            size_t index = at + 1;
            while( index < max && (carray[index] != quotation) )
            {
                if( carray[index] == '\\' )
                {
                    // skip 2 chars..
                    ++index;
                }
                ++index;
            }
            
            return index - at - 1;
        }
        
        size_t seekWhitespace( const char *carray , size_t max , size_t at )
        {
            size_t index = at;
            while( (carray[index] == ' ' || carray[index] == '\t' || carray[index] == '\n' || carray[index] == '\r' || carray[index] == ',') && index < max )
            {
                ++index;
            }
            return index - at;
        }
        
        Script parse( const std::string& text , const TypeMap& types )
        {
            Script script;
            
            const char *carray = text.c_str();
            const size_t max = text.size();
            
            if( max < 1 )
            {
                return script;
            }
            
            Token token;
            Token token2;
            size_t at = 0;
            size_t index = 0;
            size_t total = 0;
            size_t left = max;
            size_t consumed = 0;
            
            while( at < max )
            {
                auto key = carray[at];
                token.index = 0;
                token2.index = 0;
                
                left = max - at;
                consumed = 1;
                
                // any key glyphs..
                switch( key )
                {
                    // 1 letter reserved keys
                    case '{' : token.code = KI_BLOCK_BEGIN; break;
                    case '}' : token.code = KI_BLOCK_END;   break;
                    case '(' : token.code = KI_PARAM_BEGIN; break;
                    case ')' : token.code = KI_PARAM_END;   break;
                    case ';' : token.code = KI_END;         break;
                        
                    case '[' : token.code = KI_ENTITY_BEGIN;break;
                    case ']' : token.code = KI_ENTITY_END;  break;
                        
                    case '=' : token.code = KI_ASSIGN;      break;
                    case ':' : token.code = KI_ASSIGN;      break;
                    case '+' : token.code = KI_ADD;         break;
                    case '-' : token.code = KI_SUBTRACT;    break;
                    case '*' : token.code = KI_MULTIPLY;    break;
                    case '%' : token.code = KI_MODULO;      break;
                        
                    case '!' : token.code = KI_NOT;         break;
                    case '|' : token.code = KI_BAND;        break;
                    case '&' : token.code = KI_BOR;         break;
                    case '\\': token.code = KI_SPECIAL;     break;
                        
                    case '.': token.code = KI_MEMBER;       break;
                        
                    case '/' :
                    {
                        // is it a comment.. or divide, or other someting special
                        // lets assume, it is divide..
                        token.code = KI_DIVIDE;
                        if( left < 2 )
                        {
                            break;
                        }
                        
                        size_t beginning = at;
                        auto next = carray[at+1];
                        // a /*
                        if ( next == '*' )
                        {
                            // seek */
                            at += 2;
                            for(; at < max ; ++at)
                            {
                                if( ((at+1) < max) && (carray[at] == '*') && (carray[at+1] == '/') )
                                {
                                    ++at;
                                    break;
                                }
                            }
                            // lets count comments as whitespace..
                            token.code = KI_WHITESPACE;
                            consumed = (at + 1) - beginning;
                        }
                        // a //
                        else if( next == '/')
                        {
                            // seek n (newline)
                            at += 2;
                            for(; at < max ; ++at)
                            {
                                if( carray[at] == '\n' )
                                {
                                    break;
                                }
                            }
                            // lets count comments as whitespace..
                            token.code = KI_WHITESPACE;
                            consumed = (at + 1) - beginning;
                        }
                        break;
                    }
                        
                    // string
                    case '"' :
                    case '\'' :
                    {
                        consumed = seekString(carray, max, at) + 2;
                        token.code = KI_STRING;
                        token.index = script.strings.size();
                        script.strings.push_back(std::string(carray + at + 1 , consumed - 2));
                        // we increment later on with 1, so at is off by 1
                        at += consumed - 1;
                        break;
                    }
                    
                        // whitespace
                    case ' ':
                    case ',':
                    case '\t':
                    case '\n':
                    case '\r':
                    {
                        consumed = seekWhitespace(carray, max, at);
                        token.code = KI_WHITESPACE;
                        // we increment later on with 1, so at is off by 1
                        at += consumed - 1;
                        break;
                    }
                    default  : token.code = KI_UNKNOWN;     break;
                }
                if( token.code != KI_UNKNOWN )
                {
                    ++at;
                }
                
                // the key was consumed
                if( token.code != KI_UNKNOWN )
                {
                    // 2 letter combinations?
                    if( left > 1 )
                    {
                        auto key = carray[at];
                        switch ( token.code )
                        {
                            // ==
                            case KI_ASSIGN :
                            {
                                if( key == '=' )
                                {
                                    consumed = 2;
                                    token.code = KI_EQUALS;
                                    ++at;
                                }
                                break;
                            }
                            // !=
                            case KI_NOT :
                            {
                                if( key == '=' )
                                {
                                    consumed = 2;
                                    token.code = KI_NOT_EQUALS;
                                    ++at;
                                }
                                break;
                            }
                            // ||
                            case KI_BOR :
                            {
                                if( key == '|' )
                                {
                                    consumed = 2;
                                    token.code = KI_OR;
                                    ++at;
                                }
                                break;
                            }
                            // &&
                            case KI_BAND :
                            {
                                if( key == '&' )
                                {
                                    consumed = 2;
                                    token.code = KI_AND;
                                    ++at;
                                }
                                break;
                            }
                            default : break;
                        }
                    }
                }
                
                // not recognized yet.. must be 'unknown'
                if( token.code == KI_UNKNOWN )
                {
                    ++at;
                    continue;
                }
                
                // recognized
                total = at - index;
                size_t remains = total - consumed;
                if( remains > 0 )
                {
                    token2.code = recognize( (carray + index) , remains , types );
                    
                    // not recognized..
                    if( token2.code == KI_FRAGMENT )
                    {
                        token2.code = KI_FRAGMENT;
                        token2.index = script.strings.size();
                        script.strings.push_back( std::string( (carray + index) , remains ) );
                    }
                    
                    script.tokens.push_back(token2);
                }
                
                // Move the "unprocessed index"
                index = at;
                if( token.code == KI_WHITESPACE )
                {
                    continue;
                }
                script.tokens.push_back(token);
            }
            
            return script;
        }
	}
}
