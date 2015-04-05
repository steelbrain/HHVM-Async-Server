<?hh //decl
const STREAM_AWAIT_READ = 0;
const STREAM_AWAIT_READY = 0;
const STREAM_AWAIT_CLOSED = 0;
function stream_await(resource $Resource, int $Type, int $Timeout):Awaitable<int>;
