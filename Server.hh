<?hh //strict
class Server{
  public function __construct(public string $Address, public int $Port){}
  public async function Listen((function(resource):Awaitable<void>) $Callback):Awaitable<void>{
    await RescheduleWaitHandle::create(RescheduleWaitHandle::QUEUE_DEFAULT,0);
    $Socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
    socket_bind($Socket, $this->Address, $this->Port);
    socket_listen($Socket, 100);
    register_shutdown_function(function() use ($Socket){
      socket_close($Socket);
    });
    $Handles = Vector{};
    while(is_resource($Socket)){
      $Handles->add($Callback(socket_accept($Socket))->getWaitHandle());
      if($Handles->count() === 20){
        await GenVectorWaitHandle::create($Handles);
        $Handles->clear();
      }
    }
  }
  public static async function OnResponse(resource $Socket, (function(string, resource):Awaitable<void>) $Callback):Awaitable<void>{
    while (is_resource($Socket) && !feof($Socket)){
      $select = await stream_await($Socket, STREAM_AWAIT_READ, 5);
      switch ($select) {
        case STREAM_AWAIT_READY:
          await $Callback(fgets($Socket), $Socket);
          break;
        case STREAM_AWAIT_CLOSED:
        return ;
        // Do nothing for timeouts
        default:
      }
    }
  }
}
