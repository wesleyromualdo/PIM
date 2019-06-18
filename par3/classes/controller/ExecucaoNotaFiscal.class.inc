<?php

class Par3_Controller_ExecucaoNotaFiscal extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ExecucaoNotaFiscal();
    }

    public function controlarRequisicoes($requisicao, $proid)
    {
        switch ($requisicao) {
            case 'salvarNotaFiscal':
                $this->salvar($_REQUEST);
                break;
            case 'salvaritemnf':
                $this->salvaritemnf($_REQUEST);
                break;
            case 'salvarArrayItemNF':
                $this->salvarArrayItemNF($_REQUEST);
                break;
            case 'editarNotaFiscal':
                $this->editarNotaFiscal($_REQUEST['ntfid']);
                break;
            case 'removerNotaFiscal':
                $this->removerNotaFiscal($_REQUEST['ntfid']);
                break;
            case 'listarNotasFiscais':
                $this->listaNotasFiscais($proid);
                break;
            case 'listarContratosPorFornecedor':
                $this->listarContratosPorFornecedor($_POST['cnpj'], $proid, $_POST['ntfid']);
                break;
            case 'carregarComboCnpjContratos':
                $this->carregarComboCnpjContratos($proid);
                break;
        }
    }

    /**
     * Salvar nota fiscal
     * @param $dados
     */
    public function salvar($dados)
    {
        $ntfid = $dados['ntfid'];
        $proid = $dados['proid'];
        $ntfcnpjfornecedor = pega_numero($dados['ntfcnpjfornecedor']);
        $ntfvlrnota = $dados['ntfvlrnota'];
        $ntfrazaosocialfornecedor = ($dados['ntfrazaosocialfornecedor']);
        $ntfuffornecedor = $dados['ntfuffornecedor'];
        $ntfnumeronotafiscal = $dados['ntfnumeronotafiscal'];
        $ntfdata = $dados['ntfdata'];

        $_FILES['file']['name'] = ($_FILES['file']['name']); // Necessário devido ao 'charset:false' do ajax, que é necessário para mandar o arquivo no próprio ajax

        $campos = ['ntfcnpjfornecedor' => $ntfcnpjfornecedor,
            'proid' => $proid,
            'ntfrazaosocialfornecedor' =>  $ntfrazaosocialfornecedor,
            'ntfuffornecedor' => $ntfuffornecedor,
            'ntfnumeronotafiscal' => $ntfnumeronotafiscal,
            'ntfdata' => $ntfdata ,
            'ntfvlrnota' => $ntfvlrnota,
            'usucpf' => $_SESSION['usucpf']];

        if ($dados['ntfid'] > 0) {
            $campos['ntfid'] = $ntfid;
            $campos['ntfdtalteracao'] = date('Y/m/d H:i:s', time());
            $campos['ntfcpfalteracao'] = $_SESSION['usucpf'];

            if (count($_FILES['file']['type']) > 0) {
                if ($_FILES['file']['type'] != 'application/pdf') {
                    echo 'filetype';
                    exit();
                }

                $file = new FilesSimec('execucao_notafiscal', null, 'par3');
                if ($dados['arqid'] != '') {
                    $file->excluiArquivoFisico($dados['arqid']);
                }

                $file->setUpload('PAR3 - Plano de Trabalho - Execução e Acompanhamento - Nota Fiscal', null, false);
                $campos['arqid'] = $file->getIdArquivo();
                $this->model->popularDadosObjeto($campos);
                $idNF = $this->model->salvar();
                $result = $this->model->commit();
            } else {
                $this->model->popularDadosObjeto($campos);
                $idNF = $this->model->salvar();
                $result = $this->model->commit();
            }
        } else { //incluir
            if ($_FILES['file']['type'] != 'application/pdf') {
                echo 'filetype';
                exit();
            }

            $file = new FilesSimec('execucao_notafiscal', null, 'par3');
            $file->setUpload('PAR3 - Plano de Trabalho - Execução e Acompanhamento - Contrato', null, false);
            $campos['arqid'] = $file->getIdArquivo();
            $this->model->popularDadosObjeto($campos);
            $idNF = $this->model->salvar();
            $result = $this->model->commit();
        }

        $_SESSION['ntfid'] = $idNF;

        if ($result) {
            echo $idNF;
        } else {
            echo 'falha';
        }
    }

    /** Salvar item individual */
    public function salvaritemnf($dados)
    {
        $id = $this->model->salvarItemNf($dados['ntfid'], $dados['eciid'], $dados['encqtditens'], $dados['encvaloritens'], $dados['encid']);
        $this->model->salvarQuantidadeTotal($dados['ntfid'], $dados['qtdtotal']);
        $valorTotal = $this->model->calcularValorTotalNf($dados['ntfid']); //calcula valor total da nf e tras o tipo do item

        $result['encid'] = $id;
        $result['valortotalnf'] = $valorTotal['valortotal'];
        $result['tipoitem'] = $valorTotal['tipoitem'];
        echo simec_json_encode($result);
    }

    /** salvar todos os itens juntos */
    public function salvarArrayItemNF($dados)
    {
        if ($this->model->inserirArrayItem($dados['item'])) {
            echo 'sucesso';
        }
    }

    public function listaNotasFiscais($proid)
    {
        $sql = $this->model->sqlListaNotasFiscais($proid);

        $cabecalho = array('Número da nota fiscal', 'CNPJ Fornecedor', 'Razão Social', 'Data da nota fiscal', 'Quantidade de itens', 'Valor', 'Status');

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas('ntfstatus', 'arqid');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addAcao('edit', 'editarNotaFiscal');
        $listagem->setAcaoComoCondicional('remove', [['campo' => 'ntfstatus', 'op' => 'igual', 'valor' => 'A']]);
        $listagem->addAcao('download', ['func' => 'downloadNotaFiscal', 'titulo' => 'Download Nota Fiscal', 'extra-params' => ['arqid'], 'cor' => 'success']);
        $listagem->addAcao('remove', ['func' => 'removerNotaFiscal', 'outline' => 'btn-outline']);
        $listagem->addCallbackDeCampo('ntfvlrnota', 'par3_mascaraMoeda');
        $listagem->addCallbackDeCampo('ntfcnpjfornecedor', 'formatar_cnpj');
        $listagem->turnOffForm();
        $listagem->turnOnPesquisator();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    public function listarContratosPorFornecedor($cnpj, $proid, $ntfid = null)
    {
        $cnpj = pega_numero($cnpj);
        $execucaoContratoModel = new Par3_Model_ExecucaoContrato();
        $contratos = $execucaoContratoModel->buscarContratosPorCnpjEProcesso($cnpj, $proid, $ntfid);

        $return = <<<HTML
<div style="margin-top:20px;" class="alert alert-info col-md-4 col-md-offset-4 text-center nenhum-registro">Nenhum registro encontrado</div>
<br style="clear:both" />
<br />
HTML;

        $contratos[0]['valortotalitens'] = par3_mascaraMoeda($contratos[0]['valortotalitens'], false);

        $divValorTotal = <<<HTML
                <div class="form-group">
                    <label class="col-lg-2 col-md-2 col-md-offset-8 col-lg-offset-8 control-label">Valor total dos itens:</label>
                    <div class="col-lg-2">
                        <input type="text" id="valortotalitensnf" class="form-control txtright" readonly value="{$contratos[0]['valortotalitens']}">
                    </div>
                </div>
HTML;


        if ($contratos) {
            echo $divValorTotal;
            $return = Par3_Helper_AccordionHelper::listagemAccordion($contratos, ['recarregarDiv' => false, 'req' => 'listaItensContratoNotaFiscal']);
        }

        return $return;
    }

    public function listarItensPorContrato($ecoid)
    {

        echo "<div class='col-lg-2' style='padding-left: 0;'>
                <button class='btn btn-default incluirtodos' type='button' data-ecoid='{$ecoid}'  onclick='incluirTodosItensNF(this)'>Incluir todos os itens</button>
              </div>";

        $execucaoContratoModel = new Par3_Model_ExecucaoContrato();
        $itensContrato = $execucaoContratoModel->buscarItensContratoPorId($ecoid);

//        $cabecalho = array('Descrição', 'Quantidade total', 'Itens sem pagamento', 'Valor total', 'Valor unitário', 'Quantidade a ser paga', 'Valor a ser pago (R$)', 'Quantidade Detalhada', 'Detalhamento');
        $cabecalho = array('Descrição', 'Quantidade total', 'Itens sem pagamento', 'Valor total', 'Valor unitário', 'Quantidade a ser paga', 'Valor a ser pago (R$)');

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setDados($itensContrato);
        $listagem->esconderColunas('ipiid');
        $listagem->addCallbackDeCampo(['vlrtotalitens', 'aicvaloraprovado'], 'par3_mascaraMoeda');
        $listagem->turnOffForm();
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);


        echo '<script>formataValorItensNF();</script>';
    }

    public function editarNotaFiscal($ntfid)
    {
        $sql = $this->model->sqlEditNotaFiscal($ntfid);

        $result = $this->model->pegaLinha($sql);

        $_SESSION['ntfid'] = $result['ntfid'];

        ob_start();
        $this->listarContratosPorFornecedor($result['ntfcnpjfornecedor'], $result['proid'], $result['ntfid']);
        $result['listaContratos'] = ob_get_clean();
        $result['ntfcnpjfornecedor'] = formatar_cnpj($result['ntfcnpjfornecedor']);

        echo simec_json_encode($result);
    }

    public function removerNotaFiscal($ntfid)
    {
        $this->model->ntfid = $ntfid;
        $this->model->ntfstatus = 'I';
        $this->model->ntfdtinativacao = date('Y-m-d H:i:s', time());
        $this->model->ntfcpfinativacao = $_SESSION['usucpf'];

        try {
            $this->model->salvar();
            $this->model->commit();
            echo 'sucesso';
        } catch (Simec_Db_Exception $e) {
            echo 'falha';
        }
    }

    public function comboCnpjFornecedor($proid)
    {
        $sql = "SELECT DISTINCT substr(ecocnpj, 1, 2) || '.' || SUBSTR(ecocnpj, 3, 3) || '.' ||
                      substr(ecocnpj, 6, 3) || '/' || SUBSTR(ecocnpj, 9, 4) || '-' ||
                      substr(ecocnpj, 13) as codigo,
                      substr(ecocnpj, 1, 2) || '.' || SUBSTR(ecocnpj, 3, 3) || '.' ||
                      substr(ecocnpj, 6, 3) || '/' || SUBSTR(ecocnpj, 9, 4) || '-' ||
                      substr(ecocnpj, 13) as descricao FROM par3.execucao_contrato WHERE proid = {$proid} AND ecostatus = 'A'";
        return $sql;
    }

    public function carregarComboCnpjContratos($proid)
    {
        global $simec;
        $arrCnpj = $this->comboCnpjFornecedor($proid);
        echo $simec->select('ntfcnpjfornecedor', 'CNPJ Fornecedor', null, $arrCnpj, array(), array('label-size' => '4', 'input-size' => '8'));
    }

}
