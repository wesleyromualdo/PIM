<?php
/**
 * Carga de planilha com dados financeiros.
 *
 * Recebe um arquivo do tipo xlsx e insere na tabela de carga e, depois, executa
 * o processamento dos dados carregados.
 */
include_once APPRAIZ . 'includes/library/vendor/phpoffice/phpexcel/Classes/PHPExcel/IOFactory.php';
$fm = new Simec_Helper_FlashMessage('spoemendas/carga/financeiro');
if (!empty($_POST))
{
    if ('importar' == $_POST['requisicao'] && !empty($_FILES))
    {
        if (!($_FILES['cargatesouro_arquivo']['type'] == 'application/vnd.ms-excel' || $_FILES['cargatesouro_arquivo']['type'] == 'text/csv'))
        {
            $fm->addMensagem('O arquivo deve possuir a extesão CSV, de acordo com o modelo disponivel!', 'danger');
        }
        else
        {
            if (is_uploaded_file($_FILES['cargatesouro_arquivo']['tmp_name']) && UPLOAD_ERR_OK == $_FILES['cargafinanceiro_arquivo']['error'])
            {
                $fileSimec = new FilesSimec();
                $fileSimec->setUpload($_FILES['cargatesouro_arquivo']['name'], '', false);
                $arqid = $fileSimec->getIdArquivo();

                $idCarga = pegarIdentificadorCarga();
                $dados = extrairDadosCSV($fileSimec->getArquivo($arqid), $idCarga, $arqid);

                if ($dados == NAO_MODELO)
                {
                    $fm->addMensagem('O arquivo selecionado não confere com modelo disponivel', 'danger');
                    header("Location: /spoemendas/spoemendas.php?modulo=principal/cargas/financeiro/cargaTesouroGerencial&acao=A");
                    die();
                }
                if ($dados == CONTEM_LETRAS_VALOR)
                {
                    $fm->addMensagem('Campos que contem valor R$, não pode possuir letras', 'danger');
                    header("Location: /spoemendas/spoemendas.php?modulo=principal/cargas/financeiro/cargaTesouroGerencial&acao=A");
                    die();
                }
                if ($dados == 'vazio' || $dados == false)
                {
                    $fm->addMensagem('O arquivo não possui dados, Verifique o arquivo selecionado!', 'danger');
                    header("Location: /spoemendas/spoemendas.php?modulo=principal/cargas/financeiro/cargaTesouroGerencial&acao=A");
                    die();
                }
                if ($dados['bool'])
                {
                    $fm->addMensagem($dados['msg'], 'danger');
                    header("Location: /spoemendas/spoemendas.php?modulo=principal/cargas/financeiro/cargaTesouroGerencial&acao=A");
                    die();
                }

                //$ident = persisteDadosXlsx($dados, $arqid);
                $fm->addMensagem("Registros importados com sucesso. Serão Processados {$dados} Registros");

                header("Location: /spoemendas/spoemendas.php?modulo=principal/cargas/financeiro/processamentoTesouro&acao=A&carga=" . $idCarga);
                die();
            }
        }
    }
}
include APPRAIZ . 'includes/cabecalho.inc';
(new Simec_View_Breadcrumb())
    ->add('Carga Tesouro Gerencial')
    ->render();
echo $fm;
?>

<script type="text/javascript" src="../library/bootstrap-3.0.0/js/bootstrap.file-input.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
    $(function () {
        $('input[type=file]').bootstrapFileInput();
    });

    function onCargatesouroSubmit() {
        if (!$('#cargatesouro_arquivo').val()) {
            alert('O campo "arquivo" é obrigatório e não pode ser deixado em branco.');
            return false;
        }
        return true;
    }
</script>
<div class="col-lg-12">
    <?php
    $form = new Simec_View_Form('cargatesouro');
    $form
        ->addFile('Arquivo', 'arquivo', ['modelo' => 'modelos/ModeloCargaTesouro.csv', 'accept' => '.csv'])
        ->addBotoes(['limpar', 'importar'])
        ->setRequisicao('importar')
        ->render();
    ?> 
</div>
<script>
    $(document).ready(function(){
        $('#cargatesouro').on('submit',function(){
            divCarregando();
        });
    });
</script>





