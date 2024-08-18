#pragma once

template <class F, class S>
struct Pair {
	F first;
	S second;

	Pair() :
			first(),
			second() {
	}

	Pair(F p_first, const S& p_second) :
			first(p_first),
			second(p_second) {
	}
};

template <class F, class S>
bool operator==(const Pair<F, S>& pair, const Pair<F, S>& other) {
	return (pair.first == other.first) && (pair.second == other.second);
}

template <class F, class S>
bool operator!=(const Pair<F, S>& pair, const Pair<F, S>& other) {
	return (pair.first != other.first) || (pair.second != other.second);
}


template <class F, class S>
struct PairSort {
	bool operator()(const Pair<F, S> & A, const Pair<F, S> & B) const {
		if (A.first != B.first) {
			return A.first < B.first;
		}
		return A.second < B.second;
	}
};