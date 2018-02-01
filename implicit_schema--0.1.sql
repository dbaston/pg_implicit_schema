CREATE FUNCTION auto_create_schema ()
RETURNS event_trigger
AS 'MODULE_PATHNAME', 'auto_create_schema'
LANGUAGE c IMMUTABLE STRICT;

CREATE EVENT TRIGGER auto_create_schema
ON ddl_command_start
WHEN TAG IN ('CREATE TABLE', 'CREATE VIEW')
EXECUTE PROCEDURE auto_create_schema();
