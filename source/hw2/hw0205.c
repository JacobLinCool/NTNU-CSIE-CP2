// #define DEBUG
#include "hw0205.h"
#include "helper.h"
#include "myvector.h"

i32 main() {
    sVector* cartesian_a = myvector_init();
    myvector_set(cartesian_a, Cartesian, 3, 4);

    sVector* cartesian_b = myvector_init();
    myvector_set(cartesian_b, Cartesian, -2, -1);

    sVector* polar_a = myvector_init();
    myvector_set(polar_a, Polar, 3, M_PI / 2);

    sVector* polar_b = myvector_init();
    myvector_set(polar_b, Polar, 7, M_PI * 7 / 4);

    printf("> cartesian_a\n");
    myvector_print(cartesian_a, Cartesian);
    myvector_print(cartesian_a, Polar);

    printf("> cartesian_b\n");
    myvector_print(cartesian_b, Cartesian);
    myvector_print(cartesian_b, Polar);

    printf("> polar_a\n");
    myvector_print(polar_a, Cartesian);
    myvector_print(polar_a, Polar);

    printf("> polar_b\n");
    myvector_print(polar_b, Cartesian);
    myvector_print(polar_b, Polar);

    sVector* test = myvector_init();
    printf("%d ", myvector_add(test, cartesian_a, cartesian_b));
    printf("> cartesian_a + cartesian_b\n");
    myvector_print(test, Cartesian);
    myvector_print(test, Polar);

    printf("%d ", myvector_add(test, cartesian_a, polar_a));
    printf("> cartesian_a + polar_a\n");
    myvector_print(test, Cartesian);
    myvector_print(test, Polar);

    printf("%d ", myvector_add(test, polar_a, cartesian_a));
    printf("> polar_a + cartesian_a\n");
    myvector_print(test, Cartesian);
    myvector_print(test, Polar);

    printf("%d ", myvector_add(test, polar_a, polar_b));
    printf("> polar_a + polar_b\n");
    myvector_print(test, Cartesian);
    myvector_print(test, Polar);

    f64 inner = 0;

    printf("%d ", myvector_inner_product(&inner, cartesian_a, cartesian_b));
    printf("> cartesian_a dot cartesian_b\n");
    printf("inner = %lg\n", inner);

    printf("%d ", myvector_inner_product(&inner, cartesian_a, polar_a));
    printf("> cartesian_a dot polar_a\n");
    printf("inner = %lg\n", inner);

    printf("%d ", myvector_inner_product(&inner, polar_a, cartesian_a));
    printf("> polar_a dot cartesian_a\n");
    printf("inner = %lg\n", inner);

    printf("%d ", myvector_inner_product(&inner, polar_a, polar_b));
    printf("> polar_a dot polar_b\n");
    printf("inner = %lg\n", inner);

    f64 area = 0;

    printf("%d ", myvector_area(&area, cartesian_a, cartesian_b));
    printf("> cartesian_a cross cartesian_b\n");
    printf("area = %lg\n", area);

    printf("%d ", myvector_area(&area, cartesian_a, polar_a));
    printf("> cartesian_a cross polar_a\n");
    printf("area = %lg\n", area);

    printf("%d ", myvector_area(&area, polar_a, cartesian_a));
    printf("> polar_a cross cartesian_a\n");
    printf("area = %lg\n", area);

    printf("%d ", myvector_area(&area, polar_a, polar_b));
    printf("> polar_a cross polar_b\n");
    printf("area = %lg\n", area);

    sVector* parallel_a = myvector_init();
    myvector_set(parallel_a, Cartesian, 1, 1);

    sVector* parallel_b = myvector_init();
    myvector_set(parallel_b, Polar, 100, M_PI * 5 / 4);

    printf("%d ", myvector_area(&area, parallel_a, parallel_b));
    printf("> parallel_a cross parallel_b\n");
    printf("area = %lg\n", area);

    sVector* perpendicular_a = myvector_init();
    myvector_set(perpendicular_a, Cartesian, 200, 0);

    sVector* perpendicular_b = myvector_init();
    myvector_set(perpendicular_b, Polar, 300, M_PI * 1 / 2);

    printf("%d ", myvector_area(&area, perpendicular_a, perpendicular_b));
    printf("> perpendicular_a cross perpendicular_b\n");
    printf("area = %lg\n", area);

    f64 X, Y, Tx = 100000, Ty = -100000;
    myvector_cvp(&X, &Y, &Tx, &Ty, cartesian_a, polar_a);
    printf("> CVP of cartesian_a and polar_a\n");
    printf("(%lg, %lg) is closest to (%lg, %lg)\n", X, Y, Tx, Ty);
}
