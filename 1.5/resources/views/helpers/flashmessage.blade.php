@if ($message = Session::get('success'))
    <div class="alert alert-success">
        <p><i class="fa fa-check-circle"></i> {{ $message }}</p>
    </div>
@elseif($message = Session::get('warning'))
    <div class="alert alert-warning">
        <p><i class="fa fa-exclamation-triangle"></i> {{ $message }}</p>
    </div>
@elseif($message = Session::get('error'))
    <div class="alert alert-danger">
        <p><i class="fa fa-ban"></i> {{ $message }}</p>
    </div>
@elseif($message = Session::get('info'))
    <div class="alert alert-info">
        <p><i class="fa fa-info-circle"></i> {{ $message }}</p>
    </div>
@endif