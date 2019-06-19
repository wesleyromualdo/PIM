<div class="form-group {{ $errors->has('sisid') ? 'has-error' : '' }}">
    <label class="col-md-2 control-label text-right p-xxs" for="sisid">Escolha um sistema:</label>
    <div class="col-md-9">
        {!! Form::select('sisdsc', $sistemas, null, ['class' => 'form-control', 'placeholder' => 'Selecione...', 'id' => 'sisid'])!!}
    </div>
</div>
<div class="form-group {{ $errors->has('segurancaperfil') ? 'has-error' : '' }}">
    <label class="col-md-2 control-label text-right p-xxs" for="segurancaperfil">Seguranca Perfil:</label>
    <div class="col-md-9">
        <section class="btn-group" data-toggle="buttons">
            <label class="btn btn-default">
                <input id="segurancaperfil" type="radio" name="segurancaperfil" value="t">
                Sim
            </label>

            <label class="btn btn-default active">
                <input id="segurancaperfil" type="radio" name="segurancaperfil" value="f">
                Não
            </label>
        </section>
    </div>
</div>
<div class="form-group {{ $errors->has('segurancamenu') ? 'has-error' : '' }}">
    <label class="col-md-2 control-label text-right p-xxs" for="segurancamenu">Seguranca Menu:</label>
    <div class="col-md-9">
        <section class="btn-group" data-toggle="buttons">
            <label class="btn btn-default">
                <input id="segurancamenu" type="radio" name="segurancamenu" value="t">
                Sim
            </label>

            <label class="btn btn-default active">
                <input id="segurancamenu" type="radio" name="segurancamenu" value="f">
                Não
            </label>
        </section>
    </div>
</div>
<div class="form-group {{ $errors->has('perfilmenu') ? 'has-error' : '' }}">
    <label class="col-md-2 control-label text-right p-xxs" for="perfilmenu">Perfil Menu:</label>
    <div class="col-md-9">
        <section class="btn-group" data-toggle="buttons">
            <label class="btn btn-default">
                <input id="perfilmenu" type="radio" name="perfilmenu" value="t">
                Sim
            </label>

            <label class="btn btn-default active">
                <input id="perfilmenu" type="radio" name="perfilmenu" value="f">
                Não
            </label>
        </section>
    </div>
</div>
<div class="form-group {{ $errors->has('perfilusuario') ? 'has-error' : '' }}">
    <label class="col-md-2 control-label text-right p-xxs" for="perfilusuario">Perfil Usuario:</label>
    <div class="col-md-9">
        <section class="btn-group" data-toggle="buttons">
            <label class="btn btn-default">
                <input id="perfilusuario" type="radio" name="perfilusuario" value="t">
                Sim
            </label>

            <label class="btn btn-default active">
                <input id="perfilusuario" type="radio" name="perfilusuario" value="f">
                Não
            </label>
        </section>
    </div>
</div>
<div class="form-group {{ $errors->has('tipodocumento') ? 'has-error' : '' }}">
    <label class="col-md-2 control-label text-right p-xxs" for="perfilmenu">Tipo Documento:</label>
    <div class="col-md-9">
        <section class="btn-group" data-toggle="buttons">
            <label class="btn btn-default">
                <input id="tipodocumento" type="radio" name="tipodocumento" value="t">
                Sim
            </label>

            <label class="btn btn-default active">
                <input id="tipodocumento" type="radio" name="tipodocumento" value="f">
                Não
            </label>
        </section>
    </div>
</div>
<div class="form-group {{ $errors->has('estadodocumento') ? 'has-error' : '' }}">
    <label class="col-md-2 control-label text-right p-xxs" for="estadodocumento">Estado Documento:</label>
    <div class="col-md-9">
        <section class="btn-group" data-toggle="buttons">
            <label class="btn btn-default">
                <input id="estadodocumento" type="radio" name="estadodocumento" value="t">
                Sim
            </label>

            <label class="btn btn-default active">
                <input id="estadodocumento" type="radio" name="estadodocumento" value="f">
                Não
            </label>
        </section>
    </div>
</div>
<div class="form-group {{ $errors->has('acaoestadoducomento') ? 'has-error' : '' }}">
    <label class="col-md-2 control-label text-right p-xxs" for="acaoestadoducomento">Acao Estado Documento:</label>
    <div class="col-md-9">
        <section class="btn-group" data-toggle="buttons">
            <label class="btn btn-default">
                <input id="acaoestadoducomento" type="radio" name="acaoestadoducomento" value="t">
                Sim
            </label>

            <label class="btn btn-default active">
                <input id="acaoestadoducomento" type="radio" name="acaoestadoducomento" value="f">
                Não
            </label>
        </section>
    </div>
</div>
<div class="form-group {{ $errors->has('estadodocumentoperfil') ? 'has-error' : '' }}">
    <label class="col-md-2 control-label text-right p-xxs" for="estadodocumentoperfil">Estado Documento Perfil:</label>
    <div class="col-md-9">
        <section class="btn-group" data-toggle="buttons">
            <label class="btn btn-default">
                <input id="estadodocumentoperfil" type="radio" name="estadodocumentoperfil" value="t">
                Sim
            </label>

            <label class="btn btn-default active">
                <input id="estadodocumentoperfil" type="radio" name="estadodocumentoperfil" value="f">
                Não
            </label>
        </section>
    </div>
</div>
<div class="form-group {{ $errors->has('usuarioresponsabilidade') ? 'has-error' : '' }}">
    <label class="col-md-2 control-label text-right p-xxs" for="usuarioresponsabilidade">Usuario Responsabilidade:</label>
    <div class="col-md-9">
        <section class="btn-group" data-toggle="buttons">
            <label class="btn btn-default">
                <input id="usuarioresponsabilidade" type="radio" name="usuarioresponsabilidade" value="t">
                Sim
            </label>

            <label class="btn btn-default active">
                <input id="usuarioresponsabilidade" type="radio" name="usuarioresponsabilidade" value="f">
                Não
            </label>
        </section>
    </div>
</div>
<div class="form-group">
    <div class="col-sm-4 col-sm-offset-2">
        <button type="button" class="btn btn-primary" id="btn-exportar-sistema"><i class="fa fa-search"></i> Exportar</button>
    </div>
</div>