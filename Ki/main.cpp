//
//  main.cpp
//  Ki
//
//  Created by mikael korpela on 30/12/14.
//  Copyright (c) 2014 mikael korpela. All rights reserved.
//

#include <iostream>
#include "ki/ki"

class Print : public ki::FuncInterface
{
public:
    virtual ~Print()
    {
    }
    
    virtual void operator()( const ki::Parameters& args )
    {
        std::string catenated;
        
        for( auto& arg : args )
        {
            switch ( arg.type )
            {
                case ki::KI_STRING :
                {
                    catenated += arg.string;
                    break;
                }
                case ki::KI_NUMBER :
                {
                    catenated += arg.number;
                    break;
                }
                case ki::KI_BOOL :
                {
                    catenated += (arg.boolean ? "true" : "false");
                    break;
                }
                default: break;
            }
        }
        
        std::cout << catenated << std::endl;
    }
};

// print(text:"Hello world");
// moveTo(x=140,y=200);
// moveTo(x:140 y:200);
// function moveTo( var x , var y );
// function moveTo( var x[type:number] , var y[type:number] );

int main(int argc, const char * argv[])
{
    // insert code here...
    ki::Context context;
    
    context.set<Print>("print");
    
    context.execute("\nprint('hello world');");
    return 0;
}
