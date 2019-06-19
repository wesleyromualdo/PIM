    <div class="form-group {{ $errors->has('usucpf') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs">CPF:</label>
        <div class="col-md-8">
             {!! Form::text('usucpf', $usuario->usucpf, array('class' => 'form-control numCPF', 'id'=>'usucpf', 'readonly')) !!}
            <span class="text-danger">{{ $errors->first('usucpf','') }}</span>
        </div>
        <div class="col-md-2">           
            <button type="button" class="btn btn-primary"  id="btn-SIAPE"><i class="fa fa-search"></i> Consultar dados do SIAPE</button>
        </div>
    </div>

    <div class="form-group {{ $errors->has('usunome') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs">Nome:</label>
         <div class="col-md-10">
             {!! Form::text('usunome', $usuario->usunome, array('class' => 'form-control', 'id'=>'')) !!}
             <span class="text-danger">{{ $errors->first('usunome','') }}</span>
        </div>
    </div>

    <div class="form-group {{ $errors->has('usunomeguerra') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs">Apelido:</label>
         <div class="col-md-10">
             {!! Form::text('usunomeguerra', $usuario->usunomeguerra, array('class' => 'form-control','id'=>'usunomeguerra')) !!}                       
        </div>
    </div>

    <div class="form-group {{ $errors->has('ususexo') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs">Sexo:</label>
         <div class="col-md-10">
             <section class="col-md-4 btn-group" data-toggle="buttons">                                    
                    <label for="sexo_masculino" class="btn btn-default @if($usuario->ususexo == 'M')  active  @endif">
                        <input id="sexo_masculino" type="radio" name="ususexo" value="M" @if($usuario->ususexo == 'M')  checked  @endif>
                        Masculino
                    </label>
                    <label for="sexo_feminino" class="btn btn-default @if($usuario->ususexo == 'F')  active  @endif">
                        <input id="sexo_feminino" type="radio" name="ususexo" value="F"  @if($usuario->ususexo == 'F')  checked  @endif>
                        Feminino
                    </label>
                </section>
        </div>
    </div>
    
    <div class="form-group {{ $errors->has('usudatanascimento') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs">Data de nascimento:</label>
         <div class="col-md-10">
             {!! Form::text('usudatanascimento', Carbon\Carbon::parse($usuario->usudatanascimento)->format('d/m/Y'), array('class' => 'form-control datepicker','id'=>'usudatanascimento')) !!}            
        </div>
    </div>
    
    <div class="form-group {{ $errors->has('unidadeFederativa') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs">Unidade Federativa:</label>
         <div class="col-md-10">             
             {!! Form::select('unidadeFederativa', $unidadesFederativas, $usuario->regcod, ['class' => 'form-control', 'placeholder' => 'Selecione...', 'id' => 'unidadeFederativa'] )!!}
        </div>
    </div>
    
    <div class="form-group {{ $errors->has('municipio') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs">Município:</label>
         <div class="col-md-10">
             {!! Form::select('municipio', $municpiosPorUnidade, $usuario->muncod, ['class' => 'form-control', 'placeholder' => 'Selecione...', 'id' => 'municipio'] )!!}
        </div>
    </div>  

    <div class="form-group">
        <label class="col-md-2 control-label text-right p-xxs">Tipo do órgão:</label>
         <div class="col-md-10">
            {!! Form::select('tipoOrgao', $tipoOrgao, $usuario->tpocod, ['class' => 'form-control', 'placeholder' => 'Selecione...', 'id' => 'tipoOrgao'])!!}       
        </div>
    </div>  

    <div class="form-group">
        <label class="col-md-2 control-label text-right p-xxs">Órgão:</label>
         <div class="col-md-10">       
             <select class="form-control" id="orgao">
                <option value="">Selecione</select>       
            </select>
        </div>
    </div>    

    <div class="form-group">
        <label class="col-md-2 control-label text-right p-xxs">Unidade Orçamentária:</label>
         <div class="col-md-10">            
            {!! Form::select('unidadeOrcamentaria', $unidadesOrcamentarias, $usuario->unicod, ['class' => 'form-control', 'placeholder' => 'Selecione...', 'id' => '$unidadeOrcamentaria'])!!}                        
        </div>
    </div>  

    <div class="form-group">
        <label class="col-md-2 control-label text-right p-xxs">Unidade Gestora:</label>
         <div class="col-md-10">
             {!! Form::select('unidadeGestora', $unidadesGestoras, $usuario->ungcod, ['class' => 'form-control', 'placeholder' => 'Selecione...', 'id' => 'unidadeGestora'])!!}            
        </div>
    </div>

    <div class="form-group">
        <label class="col-md-2 control-label text-right p-xxs">Telefone:</label>
         <div class="col-md-9">             
             <div class="row">
                <div class="col-md-1">{!! Form::text('ddd',  $usuario->usufoneddd  , array('class' => 'form-control', 'id'=>'ddd')) !!}</div>
                <div class="col-md-4">{!! Form::text('usufonenum',  $usuario->usufonenum  , array('class' => 'form-control', 'id'=>'usufonenum')) !!}</div>                                                  
             </div>
        </div>
    </div>

    <div class="form-group {{ $errors->has('usuemail') ? 'has-error' : '' }}">
        <label class="col-md-2 control-label text-right p-xxs">E-mail:</label>
         <div class="col-md-9">
             {!! Form::text('email', $usuario->usuemail, array('class' => 'form-control', 'id'=> 'email')) !!}
             <span class="text-danger">{{ $errors->first('usuemail','') }}</span>
        </div>
    </div>

    <div class="form-group">
        <label class="col-md-2 control-label text-right p-xxs">Função / Cargo:</label>
         <div class="col-md-9">            
             {!! Form::select('cargo', $cargos, $usuario->carid, ['class' => 'form-control', 'placeholder' => 'Selecione...', 'id' => 'cargo'])!!}                                
        </div>
    </div>


    <div class="form-group">
        <label class="col-md-2 control-label text-right p-xxs">Senha:</label>
         <div class="col-md-9">
             {!! Form::checkbox('alterarSenhaPadrao','true',false, array('id'=>'alterarSenhaPadrao')) !!}
             Alterar a senha do usuário para a senha padrão: 
        </div>
    </div>


    <div class="form-group">
        <label class="col-md-2 control-label text-right p-xxs">Status geral:</label>
         <div class="col-md-9">                        
             @if($usuario->suscod == 'A')
                Ativo
            @elseif($usuario->suscod == 'P')
                Pendente
            @elseif($usuario->suscod == 'B')
                Bloqueado                
            @endif                    
        </div>
    </div>
    <div class="form-group">
        <div class="col-sm-4 col-sm-offset-2">
            <a href="{{ route('usuarioresponsabilidade.index',['usucpf' => $usuario->usucpf])}}" class="btn btn-warning" id="btn-responsabilidade-usuario">
                <i class="fa fa-pencil"></i> 
                Responsabilidade Usuário
            </a>
        </div>
    </div>
    <hr>
    <div class="form-group">
        <div class="col-sm-4 col-sm-offset-2">
            <a href="{{ route('usuario.index') }}" class="btn btn-default"><i class="fa fa-arrow-left"></i> Voltar</a>
            <button type="submit" class="btn btn-primary"><i class="fa fa-save"></i> Salvar Usuário</button>
        </div>
    </div>
    
 @section('myjsfile')
  <script>

      var usuario  = JSON.parse('{!! json_encode($usuario) !!}');
      
        function setCMBMunicipios(municipios)
        {
            var select = $('#municipio');                                    

            console.log(select);

            select.find('option').remove(); 
            select.append(new Option("Selecione...", ""));
            
            $.each(municipios, function(key,value) {
                $('<option>').val(key).text(value).appendTo(select);     
            }); 
            
        }
        
        function setCMBOrgaos(orgaos)
        {
            
            var select = $('#orgao');                                    
        
            select.find('option').remove(); 
            select.append(new Option("Selecione...", ""));
            
            $.each(orgaos, function(key,value) {              
                $('<option>').val(key).text(value).appendTo(select);     
            }); 
            
        }
        
        function setCMBUnidadesGestoras(orgaos)
        {
            
            var select = $('#unidadeGestora');                                    
        
            select.find('option').remove(); 
            select.append(new Option("Selecione...", ""));
            
            $.each(orgaos, function(key,value) {              
                $('<option>').val(key).text(value).appendTo(select);     
            }); 
            
        }
        
        
        $(function()
        {
            
            $("#btn-SIAPE").on('click', function(){                
               var cpf = '{{$usuario->usucpf}}';
               var url = 'http://' + window.location.hostname + '/geral/dadosUsuarioSIAPE.php?cpf='+cpf;
               var janela = window.open(url, '_blank', 'width=500,height=400,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
                   janela.focus();
            });           
            

        $('#unidadeFederativa').on('change', function()
        {

            var estuf =  $('#unidadeFederativa').val();

                $.ajax({
                    type: "POST",
                    url: "{{ route('territorios.getMunicipios') }}",
                    data: {estuf:estuf},
                    success: function(res){
                      setCMBMunicipios(res);
                    },
                    error: function(XMLHttpRequest, textStatus, errorThrown) {
                        console.log(XMLHttpRequest);
                    }
                });

        });
            	
	    $("#municipio").on('change', function()
            {

                var muncod =  $('#municipio').val();
                var tpocod =  $('#tipoOrgao').val();
                var estuf  =  $('#unidadeFederativa').val();
            
                //Órgão
                $.ajax({
                    type: "POST",
                    url: "{{ route('entidade.getEntidades') }}",
                    data: {muncod:muncod, tpocod:tpocod, estuf:estuf},
                    success: function(res){

                        console.log(res);

                        setCMBOrgaos(res);
                    },
                    error: function(XMLHttpRequest, textStatus, errorThrown) {
                        console.log(XMLHttpRequest);
                    }
                }); 
                
                //Unidade Gestora
                $.ajax({
                    type: "POST",
                    url: "{{ route('publico.getUnidadeGestoraPorMunCod') }}",
                    data: {muncod:muncod},
                    success: function(res){
                        setCMBUnidadesGestoras(res);
                    },
                    error: function(XMLHttpRequest, textStatus, errorThrown) {
                        console.log(XMLHttpRequest);
                    }
                });

            });
            
            $("#tipoOrgao").on('change', function(){

                var tpocod =  $('#tipoOrgao').val();                
                var estuf  =  $('#unidadeFederativa').val();

                $.ajax({
                    type: "POST",
                    url: "{{ route('entidade.getEntidades') }}",
                    data: {tpocod:tpocod,estuf:estuf},
                    success: function(res){                     
                      //console.log(res);                                         
                    }, 
                    error: function(XMLHttpRequest, textStatus, errorThrown) {
                        console.log(XMLHttpRequest);
                    }
                });

            });            
                   
        });
    </script>
@stop