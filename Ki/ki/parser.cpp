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
            
            Entry entry;
            Entry entry2;
            size_t at = 0;
            size_t index = 0;
            size_t total = 0;
            size_t left = max;
            size_t len = 0;
            
            while( at < max )
            {
                auto key = carray[at];
                entry.index = 0;
                entry2.index = 0;
                
                left = max - at;
                len = 1;
                
                // any key glyphs..
                switch( key )
                {
                    // 1 letter reserved keys
                    case '{' : entry.code = KI_BLOCK_BEGIN; break;
                    case '}' : entry.code = KI_BLOCK_END;   break;
                    case '(' : entry.code = KI_PARAM_BEGIN; break;
                    case ')' : entry.code = KI_PARAM_END;   break;
                    case ';' : entry.code = KI_END;         break;
                        
                    case '[' : entry.code = KI_ENTITY_BEGIN;break;
                    case ']' : entry.code = KI_ENTITY_END;  break;
                        
                    case '=' : entry.code = KI_ASSIGN;      break;
                    case ':' : entry.code = KI_ASSIGN;      break;
                    case '+' : entry.code = KI_ADD;         break;
                    case '-' : entry.code = KI_SUBTRACT;    break;
                    case '/' : entry.code = KI_DIVIDE;      break;
                    case '*' : entry.code = KI_MULTIPLY;    break;
                    case '%' : entry.code = KI_MODULO;      break;
                        
                    case '!' : entry.code = KI_NOT;         break;
                    case '|' : entry.code = KI_BAND;        break;
                    case '&' : entry.code = KI_BOR;         break;
                    case '\\': entry.code = KI_SPECIAL;     break;
                        
                    // string
                    case '"' :
                    case '\'' :
                    {
                        len = seekString(carray, max, at) + 2;
                        entry.code = KI_STRING;
                        entry.index = script.strings.size();
                        script.strings.push_back(String(carray + at + 1 , len - 2));
                        // we increment later on with 1, so at is off by 1
                        at += len - 1;
                        break;
                    }
                    
                        // whitespace
                    case ' ':
                    case ',':
                    case '\t':
                    case '\n':
                    case '\r':
                    {
                        len = seekWhitespace(carray, max, at);
                        entry.code = KI_WHITESPACE;
                        // we increment later on with 1, so at is off by 1
                        at += len - 1;
                        break;
                    }
                    default  : entry.code = KI_UNKNOWN;     break;
                }
                if( entry.code != KI_UNKNOWN )
                {
                    ++at;
                }
                
                // is it possibly a preprocessor command
                if( entry.code == KI_DIVIDE )
                {
                    // TODO
                    // comment blocks etc.
                }
                // is it possibly a special command
                if( entry.code == KI_SPECIAL )
                {
                    // TODO
                    // whatever you do with '\\'
                }
                
                // the key was consumed
                if( entry.code != KI_UNKNOWN )
                {
                    // 2 letter combinations?
                    if( left > 1 )
                    {
                        auto key = carray[at];
                        switch ( entry.code )
                        {
                            // ==
                            case KI_ASSIGN :
                            {
                                if( key == '=' )
                                {
                                    len = 2;
                                    entry.code = KI_EQUALS;
                                    ++at;
                                }
                                break;
                            }
                            // !=
                            case KI_NOT :
                            {
                                if( key == '=' )
                                {
                                    len = 2;
                                    entry.code = KI_NOT_EQUALS;
                                    ++at;
                                }
                                break;
                            }
                            // ||
                            case KI_BOR :
                            {
                                if( key == '|' )
                                {
                                    len = 2;
                                    entry.code = KI_OR;
                                    ++at;
                                }
                                break;
                            }
                            // &&
                            case KI_BAND :
                            {
                                if( key == '&' )
                                {
                                    len = 2;
                                    entry.code = KI_AND;
                                    ++at;
                                }
                                break;
                            }
                            default : break;
                        }
                    }
                }
                
                // not recognized yet.. must be 'unknown'
                if( entry.code == KI_UNKNOWN )
                {
                    ++at;
                    continue;
                }
                
                // recognized
                total = at - index;
                size_t remains = total - len;
                if( remains > 0 )
                {
                    entry2.code = recognize( (carray + index) , remains , types );
                    
                    // not recognized..
                    if( entry2.code == KI_FRAGMENT )
                    {
                        entry2.code = KI_FRAGMENT;
                        entry2.index = script.strings.size();
                        script.strings.push_back( String( (carray + index) , remains ) );
                    }
                    
                    script.instructions.push_back(entry2);
                }
                
                // Move the "unprocessed index"
                index = at;
                if( entry.code == KI_WHITESPACE )
                {
                    continue;
                }
                script.instructions.push_back(entry);
            }
            
            return script;
        }
	}
}
