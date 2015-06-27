#pragma once

template<typename... Args>
struct Tuple;

template<>
struct Tuple<>
{};

template<typename Head, typename... Tail>
struct Tuple<Head , Tail...> : Tuple<Tail ...>
{
	Tuple(Head _head, Tail... tail) :Tuple<Tail...>(tail...), head(_head), base(static_cast<Tuple<Tail...>&>(*this))
	{}
	Head head;
	Tuple<Tail...>& base;
};

