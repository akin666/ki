//
//  main.cpp
//  Ki
//
//  Created by mikael korpela on 30/12/14.
//  Copyright (c) 2014 mikael korpela. All rights reserved.
//

#include <iostream>
#include "ki/ki"

#include <fstream>

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


bool load( std::string path , std::string& content )
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if( file )
    {
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        content.resize(size);
        file.seekg(0, std::ios::beg);
        file.read(&content[0], size);
        
        return true;
    }
    return false;
}

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
    
    std::string content;
    
    if( load("test.ki", content) )
    {
        context.execute(content);
    }
    
    return 0;
}

/*
print(text:"Hello World");                          // internal or external print function call
function doStuff( var text , var other )            // introduce function
{
    var cppObject = new CPPObject( text: text );    // create c++ object
    cppObject.doSomething( text:other );            // call c++ function
    cppObject.setPosition(x:10 , y:20);             // call c++ function
    scene.add(object:cppObject);                    // call c++ function, add object to scene
}

// we want to introduce custom behavior to objects, through scripting
// to replace the "brains" of entities..
// functions should be assignable to variables..
 make mockup use of the language, explore all avenues, USE IT. Create realistic use cases
 and make the codes ase "tests".. after that, implement the language.

*/









