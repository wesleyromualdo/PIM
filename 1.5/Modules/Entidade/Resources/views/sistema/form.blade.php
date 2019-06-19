    <div class="form-group {{ $errors->has('pfldsc') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="pfldsc">Descrição:</label>
        <div class="col-md-10">
            {!! Form::text('sisdsc', null, array('maxlength'=>100,'placeholder' => 'Descrição','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('sisdsc','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sisurl') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sisurl">URL do Sistema:</label>
        <div class="col-md-10">
            {!! Form::textarea('sisurl', null, array('rows' => 3, 'maxlength'=> 200,'placeholder' => 'URL do Sistema','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('sisurl','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sisabrev') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sisabrev">Abreviação:</label>
        <div class="col-md-10">
            {!! Form::textarea('sisabrev', null, array('rows' => 3, 'maxlength'=> 100,'placeholder' => 'Abreviação','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('sisabrev','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sisdiretorio') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sisdiretorio">Diretório:</label>
        <div class="col-md-10">
            {!! Form::textarea('sisdiretorio', null, array('rows' => 3, 'maxlength'=> 50,'placeholder' => 'Diretório','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('sisabrev','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sisfinalidade') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sisfinalidade">Finalidade:</label>
        <div class="col-md-10">
            {!! Form::textarea('sisfinalidade', null, array('rows' => 3, 'maxlength'=> 5000,'placeholder' => 'Finalidade','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('sisabrev','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sisrelacionado') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sisrelacionado">Sistemas Relacionados:</label>
        <div class="col-md-10">
            {!! Form::textarea('sisrelacionado', null, array('rows' => 3, 'maxlength'=> 1000,'placeholder' => 'Sistemas Relacionados','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('sisabrev','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sispublico') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sispublico">Público Alvo:</label>
        <div class="col-md-10">
            {!! Form::textarea('sispublico', null, array('rows' => 3, 'maxlength'=> 1000,'placeholder' => 'Público Alvo','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('sisabrev','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sisstatus') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sisstatus">Status:</label>
        <div class="col-md-9">
            <section class="btn-group" data-toggle="buttons">
                <label for="status_ativo" class="btn btn-default @if($sistema->sisstatus == 'A')  active  @endif">
                    <input id="sisstatus" type="radio" name="sisstatus" value="A"  @if($sistema->sisstatus == 'A')  checked  @endif>
                    Ativo
                </label>
                <label for="status_inativo" class="btn btn-default @if($sistema->sisstatus == 'I')  active  @endif">
                    <input id="sisstatus" type="radio" name="sisstatus" value="I" @if($sistema->sisstatus == 'I')  checked  @endif>
                    Inativo
                </label>
            </section>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sisexercicio') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sisexercicio">Exercício:</label>
        <div class="col-md-9">
            <section class="btn-group" data-toggle="buttons">
                <label class="btn btn-default  @if($sistema->sisexercicio == true)  active  @endif">
                    <input id="sisexercicio" type="radio" name="sisexercicio" value="TRUE" @if($sistema->sisexercicio == true)  checked  @endif>
                    Sim
                </label>
                <label class="btn btn-default @if($sistema->sisexercicio == false)  active  @endif">
                    <input id="sisexercicio" type="radio" name="sisexercicio" value="FALSE" @if($sistema->sisexercicio == false)  checked  @endif>
                    Não
                </label>
            </section>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sismostra') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sismostra">Mostra?:</label>
        <div class="col-md-9">
            <section class="btn-group" data-toggle="buttons">
                <label class="btn btn-default  @if($sistema->sismostra == true)  active  @endif">
                    <input id="sismostra" type="radio" name="sismostra" value="TRUE" @if($sistema->sismostra == true) checked  @endif>
                    Sim
                </label>

                <label class="btn btn-default @if($sistema->sismostra == false)  active  @endif">
                    <input id="sismostra" type="radio" name="sismostra" value="FALSE" @if($sistema->sismostra == false) checked  @endif>
                    Não
                </label>
            </section>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sisemail') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sisemail">E-mail?:</label>
        <div class="col-md-10">
            {!! Form::textarea('sisemail', null, array('rows' => 3, 'maxlength'=> 100,'placeholder' => 'E-mail','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('sisemail','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('paginainicial') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="paginainicial">Página Inicial:</label>
        <div class="col-md-10">
            {!! Form::textarea('paginainicial', null, array('rows' => 3, 'maxlength'=> 50,'placeholder' => 'Página Inicial','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('paginainicial','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sisarquivo') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sisarquivo">Arquivo:</label>
        <div class="col-md-10">
            {!! Form::textarea('sisarquivo', null, array('rows' => 3, 'maxlength'=> 100,'placeholder' => 'Arquivo','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('sisarquivo','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sistel') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sistel">Telefone:</label>
        <div class="col-md-10">
            {!! Form::text('sistel', null, array('rows' => 3, 'maxlength'=> 50,'placeholder' => 'Telefone','class' => 'form-control telefone_com_ddd')) !!}
            <span class="text-danger">{{ $errors->first('sistel','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sisfax') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sisfax">Fax:</label>
        <div class="col-md-10">
            {!! Form::text('sisfax', null, array('rows' => 3, 'maxlength'=> 50,'placeholder' => 'Fax','class' => 'form-control telefone_com_ddd')) !!}
            <span class="text-danger">{{ $errors->first('sisfax','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sissnalertaajuda') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sissnalertaajuda">Alerta Ajuda.:</label>
        <div class="col-md-9">
            <section class="btn-group" data-toggle="buttons">
                <label class="btn btn-default @if($sistema->sissnalertaajuda == true)  active  @endif">
                    <input id="sissnalertaajuda" type="radio" name="sissnalertaajuda" value="TRUE" @if($sistema->sissnalertaajuda == true) checked  @endif>
                    Sim
                </label>

                <label class="btn btn-default  @if($sistema->sissnalertaajuda == false)  active  @endif" >
                    <input id="sissnalertaajuda" type="radio" name="sissnalertaajuda" value="FALSE" @if($sistema->sissnalertaajuda == false) checked  @endif>
                    Não
                </label>
            </section>
        </div>
    </div>
    <div class="form-group {{ $errors->has('usucpfanalista') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="usucpfanalista">CPF Analista Responsável.:</label>
        <div class="col-md-10">
            {!! Form::text('usucpfanalista', null, array('maxlength'=>14,'placeholder' => 'CPF', 'class' => 'form-control numCPF' , 'id'=>'usucpfanalista')) !!}
            <span class="text-danger">{{ $errors->first('usucpf','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('usucpfgestor') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="usucpfgestor">CPF Gestor Responsável.:</label>
        <div class="col-md-10">
            {!! Form::text('usucpfgestor', null, array('maxlength'=>14,'placeholder' => 'CPF', 'class' => 'form-control numCPF' , 'id'=>'usucpfgestor')) !!}
            <span class="text-danger">{{ $errors->first('usucpf','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sislayoutbootstrap') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sislayoutbootstrap">Layoutbootstrap?:</label>
        <div class="col-md-9">
            <section class="btn-group" data-toggle="buttons">
                <label class="btn btn-default @if($sistema->sislayoutbootstrap == true)  active  @endif">
                    <input id="sislayoutbootstrap" type="radio" name="sislayoutbootstrap" value="TRUE" @if($sistema->sislayoutbootstrap == false) checked  @endif>
                    Sim
                </label>

                <label class="btn btn-default  @if($sistema->sislayoutbootstrap == false)  active  @endif">
                    <input id="sislayoutbootstrap" type="radio" name="sislayoutbootstrap" value="FALSE" @if($sistema->sislayoutbootstrap == false) checked  @endif>
                    Não
                </label>
            </section>
        </div>
    </div>
    <div class="form-group {{ $errors->has('sisordemmenu') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="sisordemmenu">Ordem Menu:</label>
        <div class="col-md-10">
            {!! Form::textarea('sisordemmenu', null, array('rows' => 3, 'maxlength'=> 100,'placeholder' => 'Ordem Menu.','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('sisordemmenu','') }}</span>
        </div>
    </div>
    <div class="form-group {{ $errors->has('constantevirtual') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs" for="Constante Virtual">Constante virtual:</label>
        <div class="col-md-10">
            {!! Form::textarea('constantevirtual', null, array('rows' => 3, 'maxlength'=> 1000,'placeholder' => 'Constante Virtual.','class' => 'form-control')) !!}
            <span class="text-danger">{{ $errors->first('constantevirtual','') }}</span>
        </div>
    </div>
    <hr>
    <div class="form-group">
        <div class="col-sm-4 col-sm-offset-2">
            <a href="{{ route('sistema.index') }}" class="btn btn-default"><i class="fa fa-arrow-left"></i> Voltar</a>
            <button type="submit" class="btn btn-primary"><i class="fa fa-save"></i> Salvar</button>
        </div>
    </div>
