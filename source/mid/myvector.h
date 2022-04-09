#include "utils.h"

#ifndef MYVECTOR_H
#define MYVECTOR_H

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

#define IGNORE_THRESHOLD (1e-12)

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef uint8_t u8;
typedef double f64;

enum VectorType {
    Cartesian = 1,
    Polar,
} VectorType;

typedef struct _sVector {
    /**
     * 1: Cartesian coordinates
     * 2: polar coordinates
     */
    u8 type;

    union Component {
        struct _sCartesian {
            f64 x;
            f64 y;
        } c;
        struct _sPolar {
            f64 distance;
            /** radian */
            f64 angle;
        } p;
    } data;
} sVector;

/**
 * @brief Convert a vector from cartesian to polar coordinates.
 * @param src_vector The source vector.
 * @return The converted polar vector.
 */
sVector* vec_to_polar(const sVector* src_vector) {
    sVector* vector = (sVector*)malloc(sizeof(sVector));
    vector->type = Polar;

    if (src_vector->type == Cartesian) {
        vector->data.p.distance = sqrt(pow(src_vector->data.c.x, 2) + pow(src_vector->data.c.y, 2));
        vector->data.p.angle = atan2(src_vector->data.c.y, src_vector->data.c.x);

        if (vector->data.p.angle < 0) {
            vector->data.p.angle += 2 * M_PI;
        }
    }
    else {
        vector->data.p.distance = src_vector->data.p.distance;
        vector->data.p.angle = src_vector->data.p.angle;
    }

    return vector;
}

/**
 * @brief Convert a vector from polar to cartesian coordinates.
 * @param src_vector The source vector.
 * @return The converted cartesian vector.
 */
sVector* vec_to_cartesian(const sVector* src_vector) {
    sVector* vector = (sVector*)malloc(sizeof(sVector));
    vector->type = Cartesian;

    if (src_vector->type == Polar) {
        vector->data.c.x = src_vector->data.p.distance * cos(src_vector->data.p.angle);
        vector->data.c.y = src_vector->data.p.distance * sin(src_vector->data.p.angle);
    }
    else {
        vector->data.c.x = src_vector->data.c.x;
        vector->data.c.y = src_vector->data.c.y;
    }

    return vector;
}

// Memory allocation. Fill zeros to all memory block.
sVector* myvector_init() {
    sVector* vector = (sVector*)calloc(1, sizeof(sVector));
    return vector;
}

// If error , return -1; otherwise , return 0;
int myvector_set(sVector* src_vector, u8 type, f64 a, f64 b) {
    if (src_vector == NULL || type < 1 || type > 2) {
        return -1;
    }

    src_vector->type = type;
    if (type == Cartesian) {
        src_vector->data.c.x = a;
        src_vector->data.c.y = b;
    }
    else if (type == Polar) {
        if (a < 0 || b < 0 || b > 2 * M_PI) {
            return -1;
        }
        src_vector->data.p.distance = a;
        src_vector->data.p.angle = b;
    }

    return 0;
}

// Print the given vector according to the given type.
// If Cartesian , print ({x},{y}).
// If polar , print ({distance} , {theta}-pi). EX: distance=2 and angle=90 degree , print (2,0.5-pi).
// Theta must be between 0 and 2.
// If error , return -1; otherwise , return 0;
int myvector_print(const sVector* src_vector, u8 type) {
    if (src_vector == NULL || type < 1 || type > 2) {
        DBG_PRINT("myvector_print failed.\n");
        return -1;
    }

    DBG_PRINT("%s: ", type == Cartesian ? "Cartesian" : "Polar");

    if (type == Cartesian) {
        sVector* vector = vec_to_cartesian(src_vector);
        bool x_is_zero = fabs(vector->data.c.x) < IGNORE_THRESHOLD;
        bool y_is_zero = fabs(vector->data.c.y) < IGNORE_THRESHOLD;
        printf("(%lg, %lg)\n", x_is_zero ? 0 : vector->data.c.x, y_is_zero ? 0 : vector->data.c.y);
        free(vector);
    }
    else if (type == Polar) {
        sVector* vector = vec_to_polar(src_vector);
        printf("(%lg, %lg-pi)\n", vector->data.p.distance, vector->data.p.angle / M_PI);
        free(vector);
    }

    return 0;
}

// A = B + C
// A's type is set to B's type.
// If error , return -1; otherwise , return 0;
int myvector_add(sVector* pA, const sVector* pB, const sVector* pC) {
    if (pA == NULL || pB == NULL || pC == NULL) {
        DBG_PRINT("myvector_add failed.\n");
        return -1;
    }

    pA->type = Cartesian;

    sVector* vec_B = vec_to_cartesian(pB);
    sVector* vec_C = vec_to_cartesian(pC);

    pA->data.c.x = vec_B->data.c.x + vec_C->data.c.x;
    pA->data.c.y = vec_B->data.c.y + vec_C->data.c.y;

    free(vec_B);
    free(vec_C);

    if (pB->type == Polar) {
        sVector* vec_polar = vec_to_polar(pA);
        myvector_set(pA, Polar, vec_polar->data.p.distance, vec_polar->data.p.angle);
        free(vec_polar);
    }

    return 0;
}

// A = B dot C
// If error , return -1; otherwise , return 0;
int myvector_inner_product(f64* pA, const sVector* pB, const sVector* pC) {
    if (pB == NULL || pC == NULL) {
        return -1;
    }

    sVector* vec_B = vec_to_cartesian(pB);
    sVector* vec_C = vec_to_cartesian(pC);

    *pA = vec_B->data.c.x * vec_C->data.c.x + vec_B->data.c.y * vec_C->data.c.y;

    free(vec_B);
    free(vec_C);

    return 0;
}

// Get the area of the parallelogram spanned by two vectors.
// If error , return -1; otherwise , return 0;
int myvector_area(f64* pArea, const sVector* pB, const sVector* pC) {
    if (pB == NULL || pC == NULL) {
        return -1;
    }

    sVector* vec_B = vec_to_polar(pB);
    sVector* vec_C = vec_to_polar(pC);

    f64 area = INFINITY;

    area = vec_B->data.p.distance * vec_C->data.p.distance * sin(vec_B->data.p.angle - vec_C->data.p.angle);

    free(vec_B);
    free(vec_C);

    if (isnormal(area) == 0) {
        return -1;
    }

    if (area < 0) {
        area = -area;
    }

    if (area < IGNORE_THRESHOLD) {
        area = 0;
    }

    *pArea = area;

    return 0;
}

// Given a target point , find the closest point which can be combined from two given vectors with integer coefficients.
// (*pX,*pY) = m*A + n*B, m and n must be integers.
// (*pX,*pY) is the closet point to (*pTx ,*pTy)
// If error , return -1; otherwise , return 0;
int myvector_cvp(f64* pX, f64* pY, const f64* pTx, const f64* pTy, const sVector* pA, const sVector* pB) {
    if (pA == NULL || pB == NULL) {
        return -1;
    }

    sVector* vec_A = vec_to_cartesian(pA);
    sVector* vec_B = vec_to_cartesian(pB);

    sVector* closest_point = myvector_init();
    myvector_set(closest_point, Cartesian, 0, 0);
    f128 distance = INFINITY;

    f64 directions[][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };

    u32 DEPTH_MAX = 100000000;
    for (u32 depth = 0; depth < DEPTH_MAX; depth++) {
        f64 x = closest_point->data.c.x, y = closest_point->data.c.y;
        u8 updated = 0;

        for (u8 i = 0; i < 4; i++) {
            f64 test_x = x + vec_A->data.c.x * directions[i][0] + vec_B->data.c.x * directions[i][1];
            f64 test_y = y + vec_A->data.c.y * directions[i][0] + vec_B->data.c.y * directions[i][1];

            f128 test_distance = sqrt(pow(test_x - *pTx, 2) + pow(test_y - *pTy, 2));

            if (test_distance < distance) {
                distance = test_distance;
                closest_point->data.c.x = test_x;
                closest_point->data.c.y = test_y;
                updated = 1;
            }
        }

        if (updated == 0) {
            break;
        }

        DBG_PRINT("%d: (%lg, %lg)\n", depth, closest_point->data.c.x, closest_point->data.c.y);
    }

    *pX = closest_point->data.c.x;
    *pY = closest_point->data.c.y;

    free(vec_A);
    free(vec_B);
    free(closest_point);

    return 0;
}

#endif //MYVECTOR_H
