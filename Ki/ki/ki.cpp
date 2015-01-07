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
    
    Parameters readParameters( const Script& script , size_t& index )
    {
        Parameters parameters;
        
        // index should be at KI_PARAM_BEGIN
        auto token = script.tokens[index];
        if( token.code != KI_PARAM_BEGIN )
        {
            return parameters;
        }
        
        for( ++index ; index < script.tokens.size() ; ++index )
        {
            Parameter parameter;
            token = script.tokens[index];
            switch (token.code) {
                case KI_STRING:
                {
                    parameter.type = KI_STRING;
                    parameter.string = script.strings[token.index];
                    parameters.push_back( parameter );
                    break;
                }
                case KI_NUMBER:
                {
                    parameter.type = KI_NUMBER;
                    parameter.number = script.numbers[token.index];
                    parameters.push_back( parameter );
                    break;
                }
                case KI_BOOL:
                {
                    parameter.type = KI_BOOL;
                    parameter.boolean = script.booleans[token.index];
                    parameters.push_back( parameter );
                    break;
                }
                default:
                    break;
            }
        }
        
        return parameters;
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
                Parameters parameters = ki::readParameters(script , index);
                iter->second->operator()(parameters);
                
                auto token = script.tokens[index];
                if( token.code == KI_END )
                {
                    ++index;
                }
                
                continue;
            }
            /*
            switch (token.code) {
                case KI_STRING :
                case KI_FRAGMENT :
                    std::cout << ::ki::toString(token.code) << ": " << script.strings[token.index] << std::endl;
                    break;
                default:
                    std::cout << ::ki::toString(token.code) << std::endl;
                    break;
            }
            */
        }
    }
}
