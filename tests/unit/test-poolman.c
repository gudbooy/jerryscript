/* Copyright 2014-2016 Samsung Electronics Co., Ltd.
 * Copyright 2016 University of Szeged.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * Unit test for pool manager.
 */

#define JERRY_MEM_POOL_INTERNAL

#include "jmem-allocator.h"
#include "jmem-poolman.h"

#include "test-common.h"

// Iterations count
const uint32_t test_iters = 1024;

// Subiterations count
#define TEST_MAX_SUB_ITERS  1024

uint8_t *ptrs[TEST_MAX_SUB_ITERS];
uint8_t data[TEST_MAX_SUB_ITERS][JMEM_POOL_CHUNK_SIZE];

int
main ()
{
  TEST_INIT ();

  jmem_init ();

  for (uint32_t i = 0; i < test_iters; i++)
  {
    const size_t subiters = ((size_t) rand () % TEST_MAX_SUB_ITERS) + 1;

    for (size_t j = 0; j < subiters; j++)
    {
      ptrs[j] = (uint8_t *) jmem_pools_alloc ();

      if (ptrs[j] != NULL)
      {
        for (size_t k = 0; k < JMEM_POOL_CHUNK_SIZE; k++)
        {
          ptrs[j][k] = (uint8_t) (rand () % 256);
        }

        memcpy (data[j], ptrs[j], JMEM_POOL_CHUNK_SIZE);
      }
    }

    /* jmem_heap_print (false); */

    for (size_t j = 0; j < subiters; j++)
    {
      if (rand () % 256 == 0)
      {
        jmem_pools_collect_empty ();
      }

      if (ptrs[j] != NULL)
      {
        TEST_ASSERT (!memcmp (data[j], ptrs[j], JMEM_POOL_CHUNK_SIZE));

        jmem_pools_free (ptrs[j]);
      }
    }
  }

#ifdef JMEM_STATS
  jmem_pools_stats_print ();
#endif /* JMEM_STATS */

  jmem_finalize (false);

  return 0;
} /* main */
