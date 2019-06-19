    <!-- Código -->
    <div class="form-group {{ $errors->has('mnucod') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="mnucod">Código:</label>
        <div class="col-md-10">
            {!! Form::text('mnucod', null, array('maxlength'=>200,'placeholder' => 'Código','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('mnucod','') }}</span>
        </div>
    </div>
    
    <!-- Descrição -->
    <div class="form-group {{ $errors->has('mnudsc') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="mnudsc">Descrição:</label>
        <div class="col-md-10">
            {!! Form::text('mnudsc', null, array('maxlength'=>200,'placeholder' => 'Descrição','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('mnudsc','') }}</span>
        </div>
    </div>
    
    <!-- Transação -->
    <div class="form-group {{ $errors->has('mnutransacao') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="mnutransacao">Transação:</label>
        <div class="col-md-10">
            {!! Form::text('mnutransacao', null, array('maxlength'=>200,'placeholder' => 'Trsansação','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('mnutransacao','') }}</span>
        </div>
    </div>
    
    <!-- Tipo -->
    <div class="form-group" id='div-tipo'>
        <label for="tipo" class="col-md-2 control-label">Tipo:</label>
        <div class="col-md-10">

            <select name="mnutipo" id='mnutipo' class="form-control">
                <option value="" selected="selected">.. Selecione ..</option>
                <option value="1" @if(!empty($menu)) {{ $menu->mnutipo == 1 ? 'selected' : '' }} @endif>1</option>
                <option value="2" @if(!empty($menu)) {{ $menu->mnutipo == 2 ? 'selected' : '' }} @endif>2</option>
                <option value="3" @if(!empty($menu)) {{ $menu->mnutipo == 3 ? 'selected' : '' }} @endif>3</option>
                <option value="4" @if(!empty($menu)) {{ $menu->mnutipo == 4 ? 'selected' : '' }} @endif>4</option>                
            </select>
            <span class="text-danger"></span>
        </div>
    </div>
    
    <!-- Possui Submenu -->
    <div class="form-group">
        <label class="col-md-2 control-label text-right p-xxs" for="mnusnsubmenu">Possui submenu ?</label>
        <div class="col-md-10 m-t-xs">
            {{ Form::radio('mnusnsubmenu', '1' , '1') }} Sim 
            {{ Form::radio('mnusnsubmenu', '0' , '0') }} Não 
        </div>
    </div>
    
    <!-- Link -->
    <div class="form-group {{ $errors->has('mnulink') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="mnulink">Link:</label>
        <div class="col-md-10">
            {!! Form::text('mnulink', null, array('maxlength'=>200,'placeholder' => 'Link','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('mnulink','') }}</span>
        </div>
    </div>
    
    <!-- Faz parte da árvore -->
      <div class="form-group">
        <label class="col-md-2 control-label text-right p-xxs" for="mnushow">Faz parte da árvore ?</label>
        <div class="col-md-10 m-t-xs">
            {{ Form::radio('mnushow', '1' , '1') }} Sim 
            {{ Form::radio('mnushow', '0' , '0') }} Não 
        </div>
    </div>
    
    <!-- Estilo -->
    <div class="form-group {{ $errors->has('mnustile') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="mnustile">Estilo:</label>
        <div class="col-md-10">
            {!! Form::text('mnustile', null, array('maxlength'=>200,'placeholder' => '','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('mnustile','') }}</span>
        </div>
    </div>    
    
    <!-- HTML -->
    <div class="form-group" id='div-html'> 
        <label for="mnuhtml" class="col-md-2 control-label">HTML</label>
        <div class="col-md-10">
            <textarea rows="4" cols="50"  class="form-control" name="mnuhtml" id="mnuhtml"></textarea> 
            <span class="text-danger"></span>
        </div>
    </div>

    <hr>
    <div class="form-group">
        <div class="col-sm-4 col-sm-offset-2">
            <a href="{{ route('menu.index') }}" class="btn btn-default"><i class="fa fa-arrow-left"></i> Voltar</a>
            <button type="submit" class="btn btn-primary"><i class="fa fa-save"></i> Salvar</button>
        </div>
    </div>
    
 