#include <iostream>

#include "str.hh"
#include "path.hh"
#include "shared_ptr.hh"


struct A
{
	A()
	{
		std::cerr<<"constructor was called"<<std::endl;
	}
	~A()
	{
		std::cerr<<"destructor was called"<<std::endl;
	}
};

int main()
{
	std::cerr<<"this is a libmtsupport.dll test \n";
	std::cerr<<"path separator is:"<<mt::path_separator()<<std::endl;
	std::string test="123abc";
	std::cerr<<"string is: "<<test<<" reverse is:"<<mt::reverse(test)<<std::endl;
	std::string test1=" 123 ";
	std::cerr<<"string is:'"<<test1<<"' trimmed is:'"<<mt::trim(test1)<<"'"<<std::endl;
	std::string test2="123abc456abc";
	std::cerr<<"string is:"<<test2<<std::endl;
	mt::find_replace(test2,"abc","def");
	std::cerr<<"string after find_replace is:"<<test2<<std::endl;

	{
		mt::shared_ptr<A> a,b;
		a = new A();
		b=a;
	}

	//wait for user input
	std::string line;
	std::getline(std::cin,line);
}
