<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="left">Lista de funções</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
    	<div class="row">
    		<div class="col-lg-12">
            	<form method="post" name="formulario-pesquisa" id="formulario-pesquisa">
        			<input type="hidden" name="requisicao" value=""> 
        			<input type="hidden" name="enfid" value=""> 
                	<?php
                	echo $simec->radio('pesquisa[enftipo]', 'Tipo de entidade', $this->pesquisa['enftipo'], $this->arTipoEntidade, [], ['input-size'=>'8', 'label-size'=>'2']);
    				echo $simec->select('pesquisa[enfid]', 'Função entidade', $this->pesquisa['enfid'], $this->arComboEntidadesFuncaoSql, [], ['input-size'=>'4', 'label-size'=>'2']);
    				?>
        		</form>
    		</div>
    	</div>
    	<div class="row">
    		<div class="col-lg-12" style="text-align: center;">
    			<button type="button" class="btn btn-primary btn-pesquisar" data-loading-text="Pesquisando, aguarde ...">
    				<i class="fa fa-search"></i> Pesquisar
    			</button>
    			<button type="button" class="btn btn-success js-incluir-funcao">
    				<i class="fa fa-plus"></i> Incluir Função
    			</button>
            </div>
    	</div>
    	<div class="row">
    		<div class="col-lg-12">
			<?php
			$this->listaEntidadesFuncao->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
            ?>
      		</div>
    	</div>
	</div>
</div>

<script type="text/javascript">
jQuery('.glyphicon-user').parents('a').prop('title', 'Vincular tipo de representante');

function editarFuncaoEntidade(enfid)
{
	location.href = 'demandas.php?modulo=representante/cadastrarFuncao&acao=A&enfid=' + enfid;
}

jQuery('.js-incluir-funcao').click(function ()
{
	location.href = 'demandas.php?modulo=representante/cadastrarFuncao&acao=A';
});

function editarRepresentanteEntidade(enfid)
{
	location.href = 'demandas.php?modulo=representante/cadastrar&acao=A&enfid=' + enfid;
}

function excluirFuncaoEntidade(enfid)
{
    swal({
        title: "Confirmar",
        text: "Deseja apagar esta função entidade?",
        type: "success",
        html: true,
        showCancelButton: true,
        cancelButtonText: "Não",
        confirmButtonText: "Sim",
        closeOnConfirm: false
    }, function (isConfirm)
    {
        if (isConfirm) {
        	jQuery('[name="requisicao"]').val('excluir-funcao-entidade');
        	jQuery('[name="enfid"]').val(enfid);
        	jQuery('[name="formulario-pesquisa"]').submit();	
        }
        return false;
    }
);

}

jQuery('.btn-pesquisar').click(function()
{
	jQuery('[name="requisicao"]').val('pesquisar-funcao-entidade');
	jQuery('[name="formulario-pesquisa"]').submit();
});

jQuery('.btn-limpar').click(function ()
{
	location.href = location.href;
});
</script>