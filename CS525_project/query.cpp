#include "query.h"



query::query()
{
}


query::query (int type) {
	this->type = type;
	
	glGenQueries(1, &this->id);
	//std::cout << id;
}

void query::start() {
	glBeginQuery(type, id);
	inUse = true;
}

void query::end() {
	glEndQuery(type);
}

bool  query::isResultReady() {
	 glGetQueryObjectiv(id, GL_QUERY_RESULT_AVAILABLE,  &ready) ;
	 //std::cout << ready << std::endl;
	 return bool(ready);
}

bool query::isInUse() {
	return inUse;
}

unsigned int query::getResult() {
	inUse = false;
	glGetQueryObjectuiv(id, GL_QUERY_RESULT, &samples);
	return samples;
}


query::~query()
{
	//glDeleteQueries(1,&id);
}

void query::test()
{
	//std::cout << id  ;
}
