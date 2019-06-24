#include "Geometry.h"

#include <list>
#include <algorithm>

using namespace std;

namespace Geometry {
	using namespace std;

	// ��������� ��� ������ � �������������� ����������

	// ��������������� ������� ������������
	double area_triangle(point a, point b, point c)
	{
		return 0.5 * (a.x * b.y + b.x * c.y + c.x * a.y - a.y * b.x - b.y * c.x - c.y * a.x);
	}

	// ����� �� ����� ������ �� ������� � ������ ������ ������� �������?
	bool ccw(point a, point b, point c)
	{
		return area_triangle(a, b, c) > eps;
	}

	// ����� �� ����� � ��������������, ������� �������� �������� �����
	bool point_in_box(point t, point p1, point p2)
	{
		return  (abs(t.x - min(p1.x, p2.x)) <= eps || min(p1.x, p2.x) <= t.x) &&
			(abs(max(p1.x, p2.x) - t.x) <= eps || max(p1.x, p2.x) >= t.x) &&
			(abs(t.y - min(p1.y, p2.y)) <= eps || min(p1.y, p2.y) <= t.y) &&
			(abs(max(p1.y, p2.y) - t.y) <= eps || max(p1.y, p2.y) >= t.y);
	}
	// �������� ����� �� ���� �����
	point min_px(point a, point b)
	{
		return a.x < b.x || (abs(a.x - b.x) <= eps && a.y < b.y) ? a : b;
	}
	// �������� ������ �� ���� �����
	point max_px(point a, point b)
	{
		return a.x > b.x || (abs(a.x - b.x) <= eps && a.y > b.y) ? a : b;
	}
	// �������� ������ �� ���� �����
	point min_py(point a, point b)
	{
		return a.y < b.y || (abs(a.y - b.y) <= eps && a.x < b.x) ? a : b;
	}
	// �������� ������� �� ���� �����
	point max_py(point a, point b)
	{
		return a.y > b.y || (abs(a.y - b.y) <= eps && a.x > b.x) ? a : b;
	}

	// ����������� �� ����� � ��������������?
	bool point_in_polygon(point t, vector < point > p)
	{
		int i, j;
		int count = 0;
		for (i = 0; i < p.size(); ++i)
		{
			j = (i + 1) % p.size();
			if (min(p[i].y, p[j].y) < t.y && t.y <= max(p[i].y, p[j].y) &&
				ccw(min_py(p[i], p[j]), max_py(p[i], p[j]), t))
			{
				// ���� �������� ����� ����� �� ������� � ����� ��������� ������ �� �������
				// �� ����������� ���������� "���������" ����� � �������������
				++count;
			}
		}
		return count % 2;
	}

}
