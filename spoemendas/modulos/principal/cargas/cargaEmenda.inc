<?php
/**
 * Carga de planilha com dados financeiros.
 *
 * Recebe um arquivo do tipo xlsx e insere na tabela de carga e, depois, executa
 * o processamento dos dados carregados.
 */
include_once APPRAIZ . 'includes/library/vendor/phpoffice/phpexcel/Classes/PHPExcel/IOFactory.php';
include_once APPRAIZ . 'spoemendas/classes/model/Carga_emenda_siop.php';
$fm = new Simec_Helper_FlashMessage('spoemendas/carga/financeiro');
if (!empty($_POST))
{
    require_once APPRAIZ.'includes/library/simec/Importador.php';

    if ('importar' == $_POST['requisicao'] && !empty($_FILES))
    {
        if (!($_FILES['cargaemenda_arquivo']['type'] == 'application/vnd.ms-excel' || $_FILES['cargaemenda_arquivo']['type'] == 'text/csv'))
        {
            $fm->addMensagem('O arquivo deve possuir a extensão CSV, de acordo com o modelo disponivel!', 'danger');
        }
        else
        {
            if (is_uploaded_file($_FILES['cargaemenda_arquivo']['tmp_name']) && UPLOAD_ERR_OK == $_FILES['cargafinanceiro_arquivo']['error'])
            {
                $fileSimec = new FilesSimec();
                $fileSimec->setUpload($_FILES['cargaemenda_arquivo']['name'], '', false);
                $arqid = $fileSimec->getIdArquivo();
                $campos = [
                    'esfera',
                    'subfuncao',
                    'funcao',
                    'acao',
                    'localizador',
                    'programa',
                    'uo',
                    'emenda',
                    'ano_emenda',
                    'autcod',
                    'planoorcamentario',
                    'rp_atual',
                    'gnd',
                    'fonte',
                    'mapcod',
                    'dotacaoatualizada',
                    'cnpj',
                    'nome_beneficiario',
                    'uf',
                    'valor_rcl_apurada',
                    'valor_disponivel',
                    'prioridade',
                    'limiteempenhado',
                    'valorimpedido',
                    'ptres'
                ];

                function validaLinha($linha){
                    if (empty(trim($linha[7]))) {
                        return false;
                    }
                    if (strlen(trim($linha[7])) > 10) {
                        return false;
                    }
                    return true;
                }

                try {
                    $importador = new Importador(new Emenda_Model_Carga_emenda_siop());
                    $importador->setArquivo($fileSimec->getArquivo($arqid))
                        ->setCampos($campos);
                    
                    (new Emenda_Model_Carga_emenda_siop())->limpaTabela();
                    
                    foreach ($importador->carregaArquivoCSV() as $linha) {
                        $carga = new Emenda_Model_Carga_emenda_siop();
                        ver($linha);
                        if (validaLinha($linha)) {
                            foreach ($linha as $key => $valor) {
                                $atributo = $importador->getCampos()[$key];
                                if (empty($atributo)) {
                                    continue;
                                }

                                if ($atributo == 'emenda') {
                                    if (strlen($valor) > 10) {
                                        continue;
                                    }
                                    if (empty(trim($valor))) {
                                        continue;
                                    }
                                }

                                if ($atributo == 'cnpj' && $valor == '--------------') {
                                    continue;
                                }

                                if ($atributo == 'nome_beneficiario' && $valor == 'Nao Aplicavel') {
                                    continue;
                                }

                                if ($atributo == 'fonte' && strlen($valor) == 4) {
                                    $valor = substr($valor, 1, 3);
                                }

                                $carga->$atributo = $valor;
                            }
                            $carga->salvar(true, true, $campos);
                            if (!$carga->commit()) {
                                throw new Exception('Erro ao inserir o registro.');
                            }
                        }
//                        ver($carga,d);
                    }
                } catch (Exception $e) {
                    $fm->addMensagem($e->getMessage(), 'danger');
                    header("Location: /spoemendas/spoemendas.php?modulo=principal/cargas/cargaEmenda&acao=A");
                    die();
                }

                //$ident = persisteDadosXlsx($dados, $arqid);
                $fm->addMensagem("Registros importados com sucesso. Serão Processados {$dados} Registros");

                header("Location: /spoemendas/spoemendas.php?modulo=principal/cargas/cargaEmenda&acao=A");
                die();
            }
        }
    }

    if ($_POST['requisicao'] == 'job'){

    }
}

function verificaCarga(){
    global $db;
    $sql = <<<SQL
      select 1 from emenda.carga_emenda_siop limit 1
SQL;
    return $db->pegaUm($sql);
}

$temCarga = verificaCarga();

include APPRAIZ . 'includes/cabecalho.inc';
(new Simec_View_Breadcrumb())
    ->add('Carga Emenda')
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
    $form = new Simec_View_Form('cargaemenda');
    $form
        ->addFile('Arquivo', 'arquivo', ['modelo' => 'modelos/ModeloCargaTesouro.csv', 'accept' => '.csv'])
        ->addBotoes(['limpar', 'importar'])
        ->setRequisicao('importar')
        ->render();
    ?>
</div>

<?php if($temCarga){ ?>

    <div class="col-lg-12">
        <?php

        ?>
    </div>

<?php } ?>

<script>
    $(document).ready(function(){
        $('#cargatesouro').on('submit',function(){
            divCarregando();
        });
    });
</script>





