/*
 * ki
 *
 *  Created on: 19.12.2014
 *      Author: akin
 */

#include "ki"
#include "parser"
#include <iostream>

namespace ki
{
	Context::Context()
    : usertypes( KI_USERTYPES + 1 )
	{
	}
    
    Script Context::parse( const std::string& text ) const
    {
        return parser::parse( text , types );
    }
	
	void Context::execute( const std::string& text )
	{
        execute( parser::parse( text , types ) );
	}
    
    Arguments readArguments( const Script& script , size_t& index )
    {
        Arguments arguments;
        
        // index should be at KI_PARAM_BEGIN
        auto token = script.tokens[index];
        if( token.code != KI_PARAM_BEGIN )
        {
            return arguments;
        }
        
        for( ++index ; index < script.tokens.size() ; ++index )
        {
            Argument arg;
            token = script.tokens[index];
            switch (token.code) {
                case KI_STRING:
                {
                    arg.type = KI_STRING;
                    arg.string = script.strings[token.index];
                    arguments.push_back( arg );
                    break;
                }
                case KI_NUMBER:
                {
                    arg.type = KI_NUMBER;
                    arg.number = script.numbers[token.index];
                    arguments.push_back( arg );
                    break;
                }
                case KI_BOOL:
                {
                    arg.type = KI_BOOL;
                    arg.boolean = script.booleans[token.index];
                    arguments.push_back( arg );
                    break;
                }
                default:
                    break;
            }
        }
        
        return arguments;
    }
    
    void Context::execute( const Script& script )
    {
        // do whatever the script tells...
        for(size_t index = 0 ; index < script.tokens.size() ; ++index)
        {
            auto token = script.tokens[index];
            
            auto iter = custom.find(token.code);
            
            if( iter != custom.end() )
            {
                ++index;
                Arguments arguments = ki::readArguments(script , index);
                iter->second->operator()(arguments);
                
                auto token = script.tokens[index];
                if( token.code == KI_END )
                {
                    ++index;
                }
                
                continue;
            }
            
            switch (token.code) {
                case KI_STRING :
                case KI_FRAGMENT :
                    std::cout << ::ki::toString(token.code) << ": " << script.strings[token.index] << std::endl;
                    break;
                default:
                    std::cout << ::ki::toString(token.code) << std::endl;
                    break;
            }
        }
    }
}
