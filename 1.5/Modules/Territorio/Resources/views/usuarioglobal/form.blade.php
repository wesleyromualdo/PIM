    <div class="form-group {{ $errors->has('usucpf') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="usucpf">CPF (ou parte do CPF):</label>
        <div class="col-md-10">
            {!! Form::text('usucpf', null, array('maxlength'=>14,'placeholder' => 'CPF', 'class' => 'form-control numCPF' , 'id'=>'usucpf')) !!}
            <span class="text-danger">{{ $errors->first('usucpf','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('usunome') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="usunome">Nome completo (ou parte do nome):</label>
         <div class="col-md-10">
            {!! Form::text('usunome', null, array('maxlength'=>100,'placeholder' => 'Nome completo','class' => 'form-control', 'id'=>'usunome')) !!}
            <span class="text-danger">{{ $errors->first('usunome','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('regcod') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="unidadeFederativa">Unidade Federativa:</label>
         <div class="col-md-9">
            {!! Form::select('unidadeFederativa', $unidadesFederativas, null, ['class' => 'form-control', 'placeholder' => 'Selecione...', 'id' => 'unidadeFederativa'])!!}
        </div>
    </div>
    <div class="form-group {{ $errors->has('muncod') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="muncod">Município:</label>
         <div class="col-md-9">
             <select id="municipio" class="form-control"><option>Selecione...</option></select>
        </div>
    </div>

    <div class="form-group {{ $errors->has('muncod') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="muncod">Sistema:</label>
         <div class="col-md-9">
             {{ $sistema }}
        </div>
    </div>

    <div class="form-group {{ $errors->has('pflcod') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="perfil">Perfil:</label>
         <div class="col-md-9">
            {!! Form::select('perfil', $perfis, null, ['class' => 'form-control', 'placeholder' => 'Selecione...', 'id' => 'perfil'])!!}
        </div>
    </div>

    <div class="form-group {{ $errors->has('unicod') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="unicod">Unidade Orçamentária:</label>
         <div class="col-md-9">
            {!! Form::select('unidadeOrcamentaria', $unidadesOrcamentarias, null, ['class' => 'form-control', 'placeholder' => 'Selecione...', 'id' => 'unidadeOrcamentaria'])!!}
        </div>
    </div>


    <div class="form-group {{ $errors->has('suscod') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="suscod">Status geral do usuário:</label>
         <div class="col-md-9">
            
                <section class="btn-group" data-toggle="buttons">
                    <label class="btn btn-default active">
                        <input id="suscod" type="radio" name="suscod" value="" checked="checked">
                        Qualquer
                    </label>
                    <label for="status_ativo" class="btn btn-default">
                        <input id="suscod" type="radio" name="suscod" value="A">
                        Ativo
                    </label>
                    <label for="status_pendente" class="btn btn-default">
                        <input id="suscod" type="radio" name="suscod" value="P">
                        Pendente
                    </label>
                    <label for="status_bloqueado" class="btn btn-default">
                        <input id="suscod" type="radio" name="suscod" value="B">
                        Bloqueado
                    </label>
                </section>

        </div>
    </div>
    
      <div class="form-group {{ $errors->has('usuchaveativacao') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="usuchaveativacao">Já acessou ao sistema:</label>
         <div class="col-md-9">
             <section class="btn-group" data-toggle="buttons">
                <label class="btn btn-default active">
                    <input id="usuchaveativacao" type="radio" name="usuchaveativacao" value="" checked="checked">
                    Qualquer
                </label>

                <label class="btn btn-default">
                    <input id="usuchaveativacao" type="radio" name="usuchaveativacao" value="t">
                    Sim
                </label>

                <label class="btn btn-default">
                    <input id="usuchaveativacao" type="radio" name="usuchaveativacao" value="f">
                    Não
                </label>
            </section>
             
             
        </div>
    </div>
    
    
    <div class="form-group {{ $errors->has('sitperfil') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sitperfil">Situação Perfil:</label>
        <div class="col-md-9">
            <section class="btn-group" data-toggle="buttons">
                <label class="btn btn-default active" name="sitperfil">
                    <input type="radio" name="sitperfil" id="sitperfil" value="V" checked="checked">
                    Qualquer
                </label>
                <label class="btn btn-default" name="sitperfil">
                    <input type="radio" name="sitperfil" id="sitperfil" value="D">
                    Desejado
                </label>
                <label class="btn btn-default" name="sitperfil">
                    <input type="radio" name="sitperfil" id="sitperfil" value="A">
                    Atribuído
                </label>
            </section>
        </div>
    </div>      

    <hr>
    <div class="form-group">
        <div class="col-sm-4 col-sm-offset-2">
            <a href="{{ route('perfil.index') }}" class="btn btn-default"><i class="fa fa-arrow-left"></i> Voltar</a>
            <button type="button" class="btn btn-primary" id="btn-pesquisar-usuario"><i class="fa fa-search"></i> Pesquisar</button>
        </div>
    </div>