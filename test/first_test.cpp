#include <boost_ut/ut.hpp>

#include <crepuscule/crepuscule.hpp>

using namespace boost::ut;

void first_test()
{
	auto i = 5 + 4;
	expect(i == 9) << "Dummy";
}

int main()
{
	"check_add"_test = first_test;
}