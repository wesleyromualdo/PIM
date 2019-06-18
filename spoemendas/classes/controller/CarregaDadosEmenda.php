<?php

require_once APPRAIZ .'includes/library/simec/Importador.php';
include_once APPRAIZ . 'spoemendas/classes/model/Carga_emenda_siop.php';

class Spoemendas_Controller_CarregaDadosEmenda {

    const TIPO_MENSAGEM_ERROR = 'danger';
    const TIPO_MENSAGEM_WARNING = 'warning';
    const TIPO_MENSAGEM_SUCCESS = 'success';

    private $idForm;
    private $flashMessage;

    public function __construct(){
        $this->idForm = 'cargaemenda';
        $this->flashMessage = new Simec_Helper_FlashMessage('spoemendas/carga/financeiro');
        if ($_POST) {
            $this->trataRquisicao();
        }
    }

    private function trataRquisicao(){
        try {
            switch ($_POST['requisicao']) {
                case 'importar':
                    $this->trataImportacao();
                    break;
            }
        } catch (Exception $e) {
            $this->redirect($e->getMessage(), $this::TIPO_MENSAGEM_ERROR);
        }
    }

    public function getFlashMessage(){
        return $this->flashMessage->getMensagens();
    }

    public function renderForm() {
        echo "<div class=\"col-lg-12\" id=\"divProcessamento\">";
        if ($_GET['processa'] == 'S') {
            $this->renderCarga();
        }
        echo "</div>";

        echo "<div class=\"col-lg-12\" id=\"divLista\">";
        if ($_GET['processa'] != 'S') {
            $form = new Simec_View_Form($this->idForm);
            $form
                ->addFile('Arquivo', 'arquivo', ['modelo' => 'modelos/ModeloCargaTesouro.csv', 'accept' => '.csv'])
                ->addBotoes(['limpar', 'importar'])
                ->setRequisicao('importar')
                ->render();
        } else {
            $this->renderListaDadosCarga();
        }
        echo "</div>";
    }

    private function renderListaDadosCarga(){
        $dados = (new Emenda_Model_Carga_emenda_siop())
            ->recuperarTodos('emenda, ano_emenda, autcod, planoorcamentario, dotacaoatualizada, cnpj, nome_beneficiario, uf, valor_rcl_apurada, prioridade, ptres', ['ano_emenda = \''.$_SESSION['exercicio'].'\'']);

        $cabecalho = [
            'emenda',
            'ano_emenda',
            'autcod',
            'planoorcamentario',
            'dotacaoatualizada',
            'cnpj',
            'nome_beneficiario',
            'uf',
            'valor_rcl_apurada',
            'prioridade',
            'ptres'
        ];

        (new Simec_Listagem())
            ->setDados($dados)
            ->setCabecalho($cabecalho)
            ->setCampos($cabecalho)
            ->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }

    private function trataImportacao() {
        if (!($_FILES['cargaemenda_arquivo']['type'] == 'application/vnd.ms-excel' || $_FILES['cargaemenda_arquivo']['type'] == 'text/csv')) {
            throw new Exception('O arquivo deve possuir a extensão CSV, de acordo com o modelo disponivel!');
        } else {
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
                    'prioridade',
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

                $dados = 0;
                $importador = new Importador(new Emenda_Model_Carga_emenda_siop());
                $importador->setArquivo($fileSimec->getArquivo($arqid))
                    ->setOffSet(1)
                    ->setCampos($campos);

                (new Emenda_Model_Carga_emenda_siop())->limpaTabela();

                foreach ($importador->carregaArquivoCSV() as $linha) {
                    $carga = new Emenda_Model_Carga_emenda_siop();
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

                            if ($atributo == 'cnpj') {
                                if ($valor == '--------------') {
                                    continue;
                                }

                                $valor = str_replace(['.', '-', '/'], '', $valor);
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
                        $dados++;
                    }
                }

                $this->redirect("Registros importados com sucesso. Serão Processados {$dados} Registros", null, ['processa=S']);
                die();
            }
        }

    }

    private function redirect($mensagem = null, $tipo = null, $arrayParamsGet = []){
        if (!empty($mensagem)) {
            $tipo = empty($tipo) ? $this::TIPO_MENSAGEM_SUCCESS : $tipo;
            $this->flashMessage->addMensagem($mensagem, $tipo);
        }
        $paramsGet = is_array($arrayParamsGet) ? '&'.implode('&', $arrayParamsGet) : '';
        header("Location: /spoemendas/spoemendas.php?modulo=principal/cargas/emenda/carregaDadosEmenda&acao=A".$paramsGet);
    }

    private function preparaCarga(){
        return function () {
            Spo_Job_Emenda_DadosEmenda::setPredecessor('Spo_Job_Preparabase', []);
            return Simec_Job_Manager::start('Spo_Job_Emenda_DadosEmenda', [
                'exercicio' => $_SESSION['exercicio']
            ]);
        };
    }

    public function renderCarga(){
        Simec_Job_Manager::render(
            null,
            null,
            (new Simec_Job_Render($this->preparaCarga()))
                ->setTitle('Carga Emendas')
        );
    }
}