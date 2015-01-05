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
        auto instruction = script.instructions[index];
        if( instruction.code != KI_PARAM_BEGIN )
        {
            return arguments;
        }
        
        for( ++index ; index < script.instructions.size() ; ++index )
        {
            Argument arg;
            instruction = script.instructions[index];
            switch (instruction.code) {
                case KI_STRING:
                {
                    arg.type = KI_STRING;
                    arg.string = script.strings[instruction.index];
                    arguments.push_back( arg );
                    break;
                }
                case KI_NUMBER:
                {
                    arg.type = KI_NUMBER;
                    arg.number = script.numbers[instruction.index];
                    arguments.push_back( arg );
                    break;
                }
                case KI_BOOL:
                {
                    arg.type = KI_BOOL;
                    arg.boolean = script.booleans[instruction.index];
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
        for(size_t index = 0 ; index < script.instructions.size() ; ++index)
        {
            auto instruction = script.instructions[index];
            
            auto iter = custom.find(instruction.code);
            
            if( iter != custom.end() )
            {
                ++index;
                Arguments arguments = ki::readArguments(script , index);
                iter->second->operator()(arguments);
                
                auto instruction = script.instructions[index];
                if( instruction.code == KI_END )
                {
                    ++index;
                }
                
                continue;
            }
            
            switch (instruction.code) {
                case KI_STRING :
                case KI_FRAGMENT :
                    std::cout << ::ki::toString(instruction.code) << ": " << script.strings[instruction.index] << std::endl;
                    break;
                default:
                    std::cout << ::ki::toString(instruction.code) << std::endl;
                    break;
            }
        }
    }
}
