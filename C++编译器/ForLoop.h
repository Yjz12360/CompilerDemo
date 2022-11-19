#pragma once
#include<vector>
#include"Assignment.h"
#include"Definition.h"
#include"Token.h"
using namespace std;
struct ForLoop {
	vector<Definition>initDef;
	vector<Assignment>initAsg;
	vector<Assignment>loopAsg;
};