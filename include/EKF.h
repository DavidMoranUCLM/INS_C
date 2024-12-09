#ifndef EKF_H
#define EKF_H

#include "gsl/gsl_matrix.h"
#include "gsl/gsl_matrix_float.h"
#include "gsl/gsl_vector_float.h"
#include "gsl_quaternion_float.h"

typedef struct measures_s {
  float acc[3];
  float mag[3];
  float velAng[3];
} measures_t;

typedef struct EKF_work_ctx_s {
  // Estimation
  gsl_matrix_float *F;
  gsl_matrix_float *W;
  gsl_matrix_float *Q;
  // Correction
  gsl_matrix_float *K;
  gsl_matrix_float *H;
  gsl_matrix_float *S;
  gsl_matrix_float *invS;
  gsl_matrix *doubleS;
  gsl_matrix *doubleInvS;
  gsl_matrix_float *R;

  gsl_vector_float *z;
  gsl_vector_float *h;

  // Buffers temporales
  const gsl_matrix_float *I3;
  const gsl_matrix_float *I4;
  gsl_matrix_float *M1_4_6;
  gsl_matrix_float *M2_4_4;
  gsl_vector_float *v1;
  gsl_vector_float *v2;
} EKF_work_ctx_t;

typedef struct EKF_ctx_s {
  EKF_work_ctx_t *wk;
  gsl_quat_float *q_current;
  gsl_quat_float *q_prev;
  gsl_quat_float *q_est;

  gsl_matrix_float *P_prev;
  gsl_matrix_float *P_current;
  gsl_matrix_float *P_est;

  gsl_vector_float *acc;
  gsl_vector_float *mag;
  gsl_vector_float *velAng;

  gsl_vector_float *horizonRefG;
  gsl_vector_float *horizonRefMag;
  float latitude;

  float currentTime;
  float prevTime;

} EKF_ctx_t;

/**
 * @brief Initializes a EKF_ctx_t pointer in heap with a reference measurement
 *
 * @param ctx
 * @param measures
 */
void ekfInit(EKF_ctx_t *ctx, EKF_work_ctx_t *wk_ctx,
             const measures_t *measures);

/**
 * @brief Deinitializes a EKF_ctx_t pointer, freeing memory;
 *
 * @param ctx
 */
void ekfDeinit(EKF_ctx_t *ctx);

/**
 * @brief Updates the filter with new measuments of a given time
 *
 * @param ctx
 * @param measures
 * @param currentTime
 */
void ekfStep(EKF_ctx_t *ctx, const measures_t *measures,
             const float currentTime);

#ifdef TEST_ENABLE

void initWorkSpace(EKF_ctx_t *ctx);
void deInitWorkSpace(EKF_ctx_t *ctx);
void ekfUpdate(EKF_ctx_t *ctx, const measures_t *measures,
               const float currentTime);
void ekfEstimate(EKF_ctx_t *ctx);
void qEst(EKF_ctx_t *ctx);
void PEst(EKF_ctx_t *ctx);
void getF(EKF_ctx_t *ctx);
void getQ(EKF_ctx_t *ctx);
void getW(EKF_ctx_t *ctx);
void ekfCorrect(EKF_ctx_t *ctx);
void get_h(EKF_ctx_t *ctx);
int getH(EKF_ctx_t *ctx);
int getR(EKF_ctx_t *ctx);
int getS(EKF_ctx_t *ctx);
int getK(EKF_ctx_t *ctx);
void ekfNorm(EKF_ctx_t *ctx);
void ekfInitConditions(EKF_ctx_t *ctx, const measures_t *measures);
void qInitEstimate(EKF_ctx_t *ctx, const measures_t *measures);
void PInitEstimate(EKF_ctx_t *ctx);

#endif

#endif