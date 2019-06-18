<script type="text/javascript">

	function cadastroNovoDocid(dlgid){
	        var r1 = window.confirm('Você deseja cadastrar um Documento para a Diligência '+dlgid+' ?');
	        if(r1){ 
	            var url = '?modulo=principal/listaDiligencia&acao=L&dlgid='+dlgid+'&novoDoc=true';
	            janela = window.open(url, 'inserirdiligencia', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' );
	        }else{
	            return false;
	        }
	}

    function alterarDelig ( dlgid ){
        janela = window.open('?modulo=principal/cadDiligencia&acao=<?php echo $_GET['acao'] ?>&dlgid=' + dlgid, 'inserirDiligencia', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' );
        janela.focus();
    }


    function excluirDelig ( dlgid ){
        <?
        $objObras = new Obras();
        $objObras->carregarPorIdCache($obrid);

        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo 'var blockEdicao = true;';
        }else{
            echo 'var blockEdicao = false;';
        }
        ?>

        if ( confirm('Deseja apagar esta Diligência?') ){
            if(blockEdicao){
                alert('Você não pode editar os dados da Obra Vinculada.');
            }else{
                location.href = '?modulo=principal/listaDiligencia&acao=<?php echo $_GET['acao'] ?>&dlgid=' + dlgid + '&requisicao=apagar';
            }
        }
    } 
</script>