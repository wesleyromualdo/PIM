    <div class="form-group {{ $errors->has('horas') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="horas">Erros agrupados das últimas:</label>
        <div class="col-md-10">
            {!! Form::text('horas', 24, array('maxlength'=>14,'', 'class' => 'form-control' , 'id'=>'horas')) !!}
            <span class="text-danger">{{ $errors->first('horas','') }}</span>
        </div>
    </div>
    <hr>
    <div class="form-group">
        <div class="col-sm-4 col-sm-offset-2">
            <button type="button" class="btn btn-primary" id="btn-pesquisar-erros"><i class="fa fa-search"></i> Pesquisar</button></div>
    </div>