<?php

include_once APPRAIZ . 'includes/library/simec/job/Manager.php';
include_once APPRAIZ . 'includes/library/simec/job/Model.php';
include_once APPRAIZ . 'includes/library/simec/job/controller/Job.php';
include_once APPRAIZ . 'includes/library/simec/job/Render.php';
require_once APPRAIZ . 'includes/library/simec/Importador.php';

class Spo_Controller_CargaUG {

    protected $dados;

    public $fm;

    public function __construct() {
        $this->dados = $_POST ? $_POST : [];
        $this->fm = new Simec_Helper_FlashMessage('spo/cargaug');
        if ($this->dados) {
            $this->trataRequisicaoPost();
        }
    }

    public function trataRequisicaoPost(){
        if ($this->dados) {
            switch ($this->dados['requisicao']) {
                case 'importar':
                    $this->trataImportacao();
                    break;
            }
        }
    }

    public function renderForm() {
        (new Simec_View_Form('form-pesquisa'))
            ->carregarDados($this->dados['form-pesquisa'])
            ->setRequisicao('pesquisar')
            ->addCombo('Unidade Gestora', 'ungcod', $this->comboUnidadeGestora())
            ->addCombo('Unidade Orçamentária', 'unicod', $this->comboUnidade())
            ->addBotoes([
                'buscar',
                'avancado' => [
                    'label' => 'Carga de UG',
                    'class' => 'btn-info',
                    'id' => 'btn-carga-ug'
                ]
            ])
            ->setFormOff()
            ->render();
    }

    public function renderListagem() {
        $where = [];
        $dados = $this->dados['form-pesquisa'];

        if ($dados['ungcod']) {
            $where[] = "ungcod = '".$dados['ungcod']."'";
        }

        if ($dados['unicod']) {
            $where[] = "unicod = '".$dados['unicod']."'";
        }

        $sql = (new Public_Model_Unidadegestora())->recuperarTodosLista($where, true);

        (new Simec_Listagem())
            ->setQuery($sql)
            ->setCabecalho([
                'Código da UG',
                'Unidade Gestora',
                'Abreviação da UG',
                'Status da UG',
                'Código da UO',
                'Unidade Orçamentária',
                'Abreviação da UO'
            ])
            ->setFormOff()
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public function renderFormCarga(){
        if ($_GET['processamento'] != 'S') {
            (new Simec_View_Form('form-carrega-arquivo'))
                ->addFile('Arquivo', 'arquivo', ['accept' => '.csv', 'enctype'])
                ->setRequisicao('importar')
                ->addBotoes(['importar'])
                ->render();
        } else {
            Simec_Job_Manager::render(
                null,
                null,
                (new Simec_Job_Render($this->preparaCarga()))
                    ->setTitle('Carga de Unidade Gestora')
            );

            $this->fm->addMensagem('Importação realizada com sucesso');

            echo "<div id=\"div-listagem\">";
            $this->renderListagemCarga();
            echo "</div>";
        }
    }

    public function renderListagemCarga() {
        $sql = (new Spo_Model_Cargaug())->recuperarTodos('ungcod, ungdsc, ungabrev, ungstatus, unicod, unidsc, uniabrev', null, null, ['query' => true]);

        (new Simec_Listagem())
            ->setQuery($sql)
            ->setCabecalho([
                'Código da UG',
                'Descrição da UG',
                'Abreviação da UG',
                'Status da UG',
                'Código da UO',
                'Descrição da UO',
                'Abreviação da UO'
            ])
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    private function preparaCarga(){
        return function () {
            Spo_Job_Cargaug::setPredecessor('Spo_Job_Preparabase', []);
            return Simec_Job_Manager::start('Spo_Job_Cargaug', [
                'exercicio' => $_SESSION['exercicio']
            ]);
        };
    }

    public function comboUnidadeGestora() {
        return (new Public_Model_Unidadegestora())->recuperarTodosFormatoInput(
            "ungcod || ' - ' || ungdsc",
            ["ungstatus = 'A'", "(unicod like '26%' OR unicod in ('73107','74902'))", "unicod is not null", "unicod != ''"],
            'ungcod'
        );
    }

    public function comboUnidade() {
        return (new Public_Model_Unidade())->recuperarTodosFormatoInput(
            "unicod || ' - ' || unidsc",
            ["(unicod like '26%' OR unicod in ('73107','74902'))"],
            'unicod'
        );
    }

    protected function trataImportacao() {
        try {
            $file = $_FILES['form-carrega-arquivo_arquivo'];
            if (!($file['type'] == 'application/vnd.ms-excel' || $file['type'] == 'text/csv')) {
                throw new Exception('O arquivo deve possuir a extensão CSV, de acordo com o modelo disponivel!');
            } else {
                if (is_uploaded_file($file['tmp_name']) && UPLOAD_ERR_OK == $file['error']) {
                    $campos = [
                        'ungcod',                   // Código da unidade gestora
                        '',
                        'ungdsc',                   // Descrição da unidade gestora
                        'ungabrev',                 // Abreviação da unidade gestora
                        'ungstatus',                // Status da unidade gestora
                        'unicod',                   // Código da unidade orçamentária
                        'unidsc',                   // Descrição da unidade orçamentária
                        'uniabrev'                  // Abreviação da unidade orçamentária
                    ];

                    $importador = new Importador();
                    $importador->setArquivo($file['tmp_name'])
                        ->setOffSet(1);

                    (new Spo_Model_Cargaug())->limpaTabela();

                    foreach ($importador->carregaArquivoCSV(';', false) as $linha) {
                        $carga = new Spo_Model_Cargaug();
                        $pendencia = '';
                        foreach ($linha as $key => $valor) {
                            $atributo = $campos[$key];
                            if (empty($atributo)) { continue; }

                            switch ($atributo) {
                                case 'ungstatus':
                                    if ($valor == 'ATIVA') {
                                        $valor = 'A';
                                    } else {
                                        $valor = 'I';
                                    }
                                    break;

                                case 'ungabrev':
                                    if (strlen($valor) > 20) {
                                        $valor = substr($valor, 0, 20);
                                        $pendencia .= "O campo \"Abreviação da UG\" é maior que o limite de 20 caractéres. Por isto a abreviação informada no arquivo foi reduzida.</br>";
                                    }
                                    break;
                            }
                            if (!empty($pendencia)) {
                                $carga->pendencia = $pendencia;
                            }

                            $carga->$atributo = ($valor);

                        }
                        $carga->salvar();
                        if (!$carga->commit()) {
                            throw new Exception('Erro ao salvar a carga de UG');
                        }
                    }
                } else {
                    throw new Exception($this->verificaErroArquivo($file['error']));
                }
            }

            $this->fm->addMensagem('Importação realizada com sucesso');
            header('Location: spo.php?modulo=sistema/tabelaapoio/cargaug/carga&acao=A&processamento=S');
            exit();
        } catch (Exception $e) {
            $this->fm->addMensagem($e->getMessage(), Simec_Helper_FlashMessage::ERRO);
            header('Location: spo.php?modulo=sistema/tabelaapoio/cargaug/carga&acao=A');
            exit();
        }
    }

    private function verificaErroArquivo($codigo){
        $mensagem = '';
        switch ($codigo) {
            case UPLOAD_ERR_INI_SIZE:
                $mensagem = 'O arquivo enviado excede o limite definido na diretiva upload_max_filesize do php.ini';
                break;

            case UPLOAD_ERR_FORM_SIZE:
                $mensagem = 'O arquivo excede o limite definido em MAX_FILE_SIZE no formulário HTML.';
                break;

            case UPLOAD_ERR_PARTIAL:
                $mensagem = 'O upload do arquivo foi feito parcialmente.';
                break;

            case UPLOAD_ERR_NO_FILE:
                $mensagem = 'Nenhum arquivo foi enviado.';
                break;

            case UPLOAD_ERR_NO_TMP_DIR:
                $mensagem = 'Pasta temporária ausênte.';
                break;

            case UPLOAD_ERR_CANT_WRITE:
                $mensagem = 'Falha em escrever o arquivo em disco.';
                break;

            case UPLOAD_ERR_EXTENSION:
                $mensagem = 'Uma extensão do PHP interrompeu o upload do arquivo. O PHP não fornece uma maneira de determinar qual extensão causou a interrupção. Examinar a lista das extensões carregadas com o phpinfo() pode ajudar.';
                break;
        }
        return $mensagem;
    }

}