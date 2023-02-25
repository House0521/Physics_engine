#ifndef RK4_H
#define RK4_H

template <class Cla, class T>	//T is float, double, vector......
T rk4(T y, float h, Cla* obj, T (Cla::*f)(float)) {	//h = dt
	T k1, k2, k3, k4, y_next;

	k1 = (obj->*f)(0.f);
	k2 = (obj->*f)(h / 2.f);
	k3 = (obj->*f)(h / 2.f);
	k4 = (obj->*f)(h);
	y_next = y + h / 6.f * (k1 + 2.f*k2 + 2.f*k3 + k4);

	return y_next;
}

template <class T>	//T is float, double, vector......
T rk4(T y, float h, T(* f)(float)) {	//h = dt
	T k1, k2, k3, k4, y_next;

	k1 = f(0.f);
	k2 = f(h / 2.f);
	k3 = f(h / 2.f);
	k4 = f(h);
	y_next = y + h / 6.f * (k1 + 2.f * k2 + 2.f * k3 + k4);

	return y_next;
}


#endif	// RK4_H
