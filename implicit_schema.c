/* 
 * Copyright 2018 Daniel Baston
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "postgres.h"
#include "executor/spi.h"
#include "commands/event_trigger.h"
#include "nodes/parsenodes.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(auto_create_schema);
Datum
auto_create_schema(PG_FUNCTION_ARGS)
{
  EventTriggerData* trigdata;
  CreateStmt* stmt;
  RangeVar* rng;
  char* schema;
  char create_schema_query[64+NAMEDATALEN];
  int ret;

  if (!CALLED_AS_EVENT_TRIGGER(fcinfo))
    elog(ERROR, "auto_create_schema can only be called as an event trigger.");

  trigdata = (EventTriggerData*) fcinfo->context;
  stmt = (CreateStmt*) trigdata->parsetree;
  rng = stmt->relation;
  schema = rng->schemaname;

  /* Check if schema was specified */
  if (!schema)
    PG_RETURN_NULL();

  snprintf(create_schema_query,
      64+NAMEDATALEN,
      "CREATE SCHEMA IF NOT EXISTS \"%s\"",
      schema);

  if ((ret = SPI_connect()) < 0)
    elog(ERROR, "SPI_connect returned %d", ret);

  ret = SPI_exec(create_schema_query, 1);

  if (ret < 0)
    elog(ERROR, "SPI_exec returned %d", ret);

  SPI_finish();

  PG_RETURN_NULL();
}

