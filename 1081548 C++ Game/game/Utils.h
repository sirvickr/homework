#pragma once

#include <string>
#include <ostream>

// ������������� ����������� ���������
bool initializeGraphics();
// ������������ ����������� ���������
void finalizeGraphics();
// ����� ������ �� �����
void logSDLError(std::ostream &os, const std::string &msg);
