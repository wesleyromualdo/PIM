    <div class="form-group {{ $errors->has('pfldsc') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="pfldsc">Descrição:</label>
        <div class="col-md-10">
            {!! Form::text('pfldsc', null, array('maxlength'=>200,'placeholder' => 'Descrição','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('pfldsc','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('pflfinalidade') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="pflfinalidade">Finalidade:</label>
        <div class="col-md-10">
            {!! Form::textarea('pflfinalidade', null, array('rows' => 3, 'maxlength'=> 1000,'placeholder' => 'Finalidade','class' => 'form-control')) !!}        
            <span class="text-danger">{{ $errors->first('pflfinalidade','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('pflnivel') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="pflnivel">Nível:</label>
        <div class="col-md-2">
            {!! Form::text('pflnivel', null, array('maxlength'=>2,'placeholder' => '','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('pflnivel','') }}</span>
        </div>
    </div>
<?php // dd($perfil) ?>
    <div class="form-group">
        <label class="col-md-2 control-label text-right p-xxs" for="pflinddelegar">Pode Delegar:</label>
        <div class="col-md-10 m-t-xs">
            {{ Form::radio('pflinddelegar', '1' , '1') }} Sim 
            {{ Form::radio('pflinddelegar', '0' , '0') }} Não 
        </div>
    </div>
    <div class="form-group">
        <label class="col-md-2 control-label text-right p-xxs" for="pflsuperuser">Super Usuário:</label>
        <div class="col-md-10 m-t-xs">
            {{ Form::radio('pflsuperuser', '1' , '1') }} Sim 
            {{ Form::radio('pflsuperuser', '0' , '0') }} Não 
        </div>
    </div>
    <div class="form-group">
        <label class="col-md-2 control-label text-right p-xxs" for="pflsuporte">Suporte:</label>
        <div class="col-md-10 m-t-xs">
            {{ Form::radio('pflsuporte', '1' , '1') }} Sim 
            {{ Form::radio('pflsuporte', '0' , '0') }} Não 
        </div>
    </div>
    <div class="form-group">
        <label class="col-md-2 control-label text-right p-xxs" for="pflsncumulativo">Cumulativo:</label>
        <div class="col-md-10 m-t-xs">
            {{ Form::radio('pflsncumulativo', '1' , '1') }} Sim 
            {{ Form::radio('pflsncumulativo', '0' , '0') }} Não 
        </div>
    </div>
    <input type="hidden" name="sisid" id="sisid" value="{{$sisid}}">
    <hr>
    <div class="form-group">
        <div class="col-sm-4 col-sm-offset-2">
            <a href="{{ route('perfilglobal.index') }}" class="btn btn-default"><i class="fa fa-arrow-left"></i> Voltar</a>
            <button type="submit" class="btn btn-primary"><i class="fa fa-save"></i> Salvar</button>
        </div>
    </div>
