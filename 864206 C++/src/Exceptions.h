#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

class TooManyInstances {
public:
	TooManyInstances(size_t maxCount) : maxCount(maxCount) {
	}
	inline size_t MaxCount() const {
		return maxCount;
	}
private:
	size_t maxCount;
};

#endif /* EXCEPTIONS_H_ */
