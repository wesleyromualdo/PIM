<?php
/**
 * Carga de planilha com dados financeiros.
 *
 * Recebe um arquivo do tipo xlsx e insere na tabela de carga e, depois, executa
 * o processamento dos dados carregados.
 */
$fm = new Simec_Helper_FlashMessage('spoemendas/carga/segov');
require_once APPRAIZ . 'www/spoemendas/_funcoes.php';
require_once APPRAIZ . 'includes/library/simec/Importador/Segov.inc';
if (!empty($_POST))
{
    if (
        $_POST['requisicao'] == 'carregar'
        && !empty($_FILES)
        && !in_array($_FILES['segov_arquivo']['type'], ['text/csv', 'application/vnd.ms-excel'])
    )
    {
        $fm->addMensagem('O arquivo deve possuir a extensão CSV, de acordo com o modelo disponivel!', 'danger');
    }
    elseif (
        is_uploaded_file($_FILES['segov_arquivo']['tmp_name'])
        && UPLOAD_ERR_OK == $_FILES['segov_arquivo']['error']
    )
    {
        try
        {
            $registros = 0;

            $model = new Spoemendas_Model_Cargasegov();
            $campos = $model->getNomeCampos();

            $campos[30] = 'casempenhado';
            $campos[31] = 'caspago';
            $campos[32] = 'casvlrautorizado';
            $campos[33] = 'casdtaautorizacao';
            $campos[34] = 'hcaid';
            unset($campos[35]);

            $importador = new Importador_Segov($model);
            $tamanhoColunas = $model->getTamanhoColunas();
            $quantidadeColunas = $model->getQuantidadeColunas();
            $smUpload = new Simec_Upload($_FILES['segov_arquivo']);
            $linhas = $smUpload->getStreamFile(true);
            $historico = new Spoemendas_Model_CargasegovHistorico();
            $idHistorico = $historico->gerarHistorico();
            $delimitador = ',';

            if(count(str_getcsv($linhas[0], $delimitador)) != count($campos) - 1) {
                $delimitador = ';';
            }
            if (count(str_getcsv($linhas[0], $delimitador)) != count($campos) - 1){
                throw new Exception('O arquivo selecionado não confere com modelo disponivel');
            }

            $carga = new Spoemendas_Model_Cargasegov();
            foreach ($linhas as $chave => $linha)
            {
                if ($chave == 0)
                {
                    continue;
                }

                $linhas[$chave] = str_getcsv($linhas[$chave], $delimitador, '"');

                foreach ($linhas[$chave] as $key => $val) {
                    $importador->validaColuna($linhas[$chave], $key, $chave, $delimitador);
                    if ($key == 29 || $key == 30 || $key == 31 || $key == 32)
                    {
                        if(';' == $delimitador) {
                            $linhas[$chave][$key] = trim(removeMascaraMoeda($val));
                        }
                        if ($linhas[$chave][$key] == 0)
                        {
                            $linhas[$chave][$key] = "'" . $linhas[$chave][$key] . "'";
                        }
                    }
                    $linhas[$chave][34] = $idHistorico;
                }
                $linhas[$chave] = array_combine($campos, $linhas[$chave]);
                $registros++;
                $carga->clearDados();
                $carga->popularDadosObjeto($linhas[$chave]);
                $carga->salvar();
            }
            if (!$carga->commit())
            {
                throw new Exception('Erro ao inserir o registro.');
            }
            $fm->addMensagem("Registros importados com sucesso. Serão processados {$registros} registros");
            header("Location: /spoemendas/spoemendas.php?modulo=principal/cargas/segov/processamento&acao=A&carga=" . $carga->hcaid);
            die();
        } catch (Exception $e)
        {
            $fm->addMensagem($e->getMessage(), 'danger');
            header("Location: /spoemendas/spoemendas.php?modulo=principal/cargas/segov/carga&acao=A");
            die();
        }
    }
}
include APPRAIZ . 'includes/cabecalho.inc';
(new Simec_View_Breadcrumb())
    ->add('Carga SEGOV')
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
        if (!$('#segov_arquivo').val()) {
            alert('O campo "arquivo" é obrigatório e não pode ser deixado em branco.');
            return false;
        }
        return true;
    }
</script>
<div class="col-lg-12">
    <?php
    $form = new Simec_View_Form('segov');
    $form
        ->addFile('Arquivo', 'arquivo', ['modelo' => 'modelos/Modelo upload - Resposta SEGOV.csv', 'accept' => '.csv'])
        ->addBotoes(['limpar', 'importar' => ['label' => 'Carregar']])
        ->setRequisicao('carregar')
        ->render();
    ?>
</div>
<script>
    $(document).ready(function () {
        $('#cargatesouro').on('submit', function () {
            divCarregando();
        });
    });
</script>