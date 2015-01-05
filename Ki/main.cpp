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
    
    virtual void arg( size_t index , const std::string& text )
    {
    }
    
    virtual void arg( size_t index , bool state )
    {
    }
    
    virtual void arg( size_t index , ki::Number number )
    {
    }
    
    virtual void operator()()
    {
        std::cout << "PRINt called" << std::endl;
    }
};

int main(int argc, const char * argv[])
{
    // insert code here...
    std::cout << "Hello, World!\n";
    ki::Context context;
    
    context.set<Print>("print");
    
    context.execute("");
    return 0;
}
