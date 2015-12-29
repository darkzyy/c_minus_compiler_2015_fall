float Fabs(float xxx) {
    return -1.0 * xxx;
}

float Fsqrt(float x) {
    float dt, t = 2.0;

    dt = (x / t - t) / 2.0;
    t = t + dt;


    dt = (x / t - t) / 2.0;
    t = t + dt;


    dt = (x / t - t) / 2.0;
    t = t + dt;


    dt = (x / t - t) / 2.0;
    t = t + dt;


    dt = (x / t - t) / 2.0;
    t = t + dt;


    dt = (x / t - t) / 2.0;
    t = t + dt;

    return t;
}

int main() {
    float a = 0.5,
          b = 0.5,
          c = -0.5;
    float rs[2];

    float dt2 = b * b - 4.0 * a * c;
    float sqrt_dt = Fsqrt(dt2);

    float x1 = (-b + sqrt_dt) / (a * 2.0);
    float x2 = (-b - sqrt_dt) / (a * 2.0);

    float x1_ans = 0.618;
    float x2_ans = -1.618;

    rs[0] = (Fabs(x1_ans - x1));
    rs[1] = (Fabs(x2_ans - x2));

    return 0;
}
