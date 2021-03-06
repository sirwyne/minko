/*
Copyright (c) 2013 Aerys

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "ProviderTest.hpp"

using namespace minko;
using namespace minko::data;

TEST_F(ProviderTest, Create)
{
	try
	{
		auto p = Provider::create();
	}
	catch (...)
	{
		ASSERT_TRUE(false);
	}
}

TEST_F(ProviderTest, CreateCopy)
{
	try
	{
		auto p1 = Provider::create();

		p1->set("foo", 42);

		auto p2 = Provider::create(p1);

		ASSERT_EQ(p2->get<int>("foo"), 42);
	}
	catch (...)
	{
		ASSERT_TRUE(false);
	}
}

TEST_F(ProviderTest, TestInt)
{
	auto provider = Provider::create();

	provider->set("foo", 42);

	ASSERT_EQ(provider->get<int>("foo"), 42);
}

TEST_F(ProviderTest, TestUint)
{
	auto provider = Provider::create();
	uint v = 42;

	provider->set("foo", v);

	ASSERT_EQ(provider->get<uint>("foo"), v);
}

TEST_F(ProviderTest, TestFloat)
{
	auto provider = Provider::create();
	float v = 42;

	provider->set("foo", v);

	ASSERT_EQ(provider->get<float>("foo"), v);
}

TEST_F(ProviderTest, PropertyAdded)
{
	auto p = Provider::create();
	auto v = 0;
	auto _ = p->propertyAdded()->connect(
		[&](Provider::Ptr provider, const std::string& propertyName)
		{
			if (provider == p && propertyName == "foo")
				v = provider->get<int>("foo");
		}
	);

	p->set("foo", 42);

	ASSERT_EQ(v, 42);
}

TEST_F(ProviderTest, PropertyRemoved)
{
	auto p = Provider::create();
	auto v = 0;
	auto _ = p->propertyRemoved()->connect(
		[&](Provider::Ptr provider, const std::string& propertyName)
		{
			if (provider == p && propertyName == "foo")
				v = 42;
		}
	);

	p->set("foo", 42);
	p->unset("foo");

	ASSERT_EQ(v, 42);
}

TEST_F(ProviderTest, ValueChanged)
{
	auto p = Provider::create();
	auto v = 0;
	auto _ = p->propertyValueChanged()->connect(
		[&](Provider::Ptr provider, const std::string& propertyName)
		{
			if (provider == p && propertyName == "foo")
				v = provider->get<int>("foo");
		}
	);

	p->set("foo", 42);

	ASSERT_EQ(v, 42);
}

//TEST_F(ProviderTest, ValueChangedNot)
//{
//	auto p = Provider::create();
//	auto v = 0;
//
//	p->set("foo", 42);
//
//	auto _ = p->propertyValueChanged()->connect(
//		[&](Provider::Ptr provider, const std::string& propertyName)
//		{
//			if (provider == p && propertyName == "foo")
//				v = provider->get<int>("foo");
//		}
//	);
//
//	p->set("foo", 42);
//
//	ASSERT_NE(v, 42);
//}

TEST_F(ProviderTest, Swap)
{
	auto p = Provider::create();
	auto vFoo = 0;
	auto vBar = 0;

	p->set("foo", 42);
	p->set("bar", 24);

	auto _ = p->propertyValueChanged()->connect(
		[&](Provider::Ptr provider, const std::string& propertyName)
		{
			if (provider == p && propertyName == "foo")
				vFoo = p->get<int>("foo");
			if (provider == p && propertyName == "bar")
				vBar = p->get<int>("bar");
		}
	);

	p->swap("foo", "bar");

	ASSERT_EQ(vFoo, 24);
	ASSERT_EQ(vBar, 42);
}
