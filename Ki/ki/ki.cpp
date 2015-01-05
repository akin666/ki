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
    
    void Context::execute( const Script& script )
    {
        // do whatever the script tells...
        for(int index = 0 ; index < script.instructions.size() ; ++index)
        {
            auto instruction = script.instructions[index];
            
            auto iter = custom.find(instruction.code);
            
            if( iter != custom.end() )
            {
                iter->second->operator()();
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
