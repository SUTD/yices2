/*
 * Parameters for SAT solver heuristics
 */

#ifndef __SAT_PARAMETERS_H
#define __SAT_PARAMETERS_H

#include <limits.h>


/*
 * Activity threshold and constants for decision heuristic
 * and for recording clause activity.
 */
#define VAR_DECAY_FACTOR         0.95
#define CLAUSE_DECAY_FACTOR      0.999
#define VAR_RANDOM_FACTOR        0.02

#define VAR_ACTIVITY_THRESHOLD        (1e100)
#define INV_VAR_ACTIVITY_THRESHOLD    (1e-100)

#define CLAUSE_ACTIVITY_THRESHOLD     (1e20f)
#define INV_CLAUSE_ACTIVITY_THRESHOLD (1e-20f)

#define INIT_VAR_ACTIVITY_INCREMENT     1.0
#define INIT_CLAUSE_ACTIVITY_INCREMENT  1.0

/*
 * Minimal number of conflicts between two successive calls to 
 * simplify_clause_database
 */
#define SIMPLIFY_CONFLICT_THRESHOLD    2000

/*
 * Restart parameters
 */
// #define INITIAL_RESTART_THRESHOLD  100  // number of conflicts before the first restart
#define INITIAL_RESTART_THRESHOLD  50  // number of conflicts before the first restart
#define MINISAT_RESTART_FACTOR 1.5      // increase factor for the restart threshold

/*
 * Heuristic for deletion of half the learned clauses
 * - changed on 2013/09/19: use linear rather than geometric increment
 * - first reduce is when the number of learned clause reaches INIT_REDUCE_THRESHOLD
 * - then after every call to reduce, the threshold is increased by INCR_REDUCE_THRESHOLD
 */
#define INITIAL_REDUCE_THRESHOLD 10000
#define INCR_REDUCE_THRESHOLD  5000

// older version: not used anymore
#define MIN_REDUCE_THRESHOLD 1000       // minimum initial threshold
#define MINISAT_REDUCE_FACTOR 1.1       // increase factor for the threshold

/*
 * Picosat-style restart and reduce
 */
// #define RESTART_FACTOR 1.1
#define RESTART_FACTOR 1.05
#define REDUCE_FACTOR  1.05
#define MAX_DTHRESHOLD 1000000

/*
 * Luby-style restart
 */
#define LUBY_INTERVAL 20

/*
 * Parameters for removing irrelevant learned clauses
 * (zchaff-style).
 */
#define TAIL_RATIO 16
#define HEAD_ACTIVITY 500
#define TAIL_ACTIVITY 10
#define HEAD_RELEVANCE 6
#define TAIL_RELEVANCE 45
#define DELETION_PERIOD 5000


#endif /* __SAT_PARAMETERS_H */
