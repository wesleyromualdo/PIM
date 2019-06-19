<?php

class Par3_Controller_ExecucaoContrato extends Controle
{

    protected $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ExecucaoContrato();
    }

    public function controlarRequisicoes($requisicao, $proid, $dadosProcesso)
    {
        switch ($requisicao) {
            case 'editarContrato':
                $this->editarContrato($_POST);
                break;
            case 'removerContrato':
                $this->removerContrato($_POST['ecoid']);
                break;
            case 'salvarContrato':
                $this->salvarContrato($_POST);
                break;
            case 'atualizarListaContratos':
                $this->listaContratos($proid);
                break;
            case 'listaContratosItem':
                $this->listaContratosItem($_POST['aicid']);
                break;
            case 'salvaritem':
                $this->salvarItem($_POST);
                break;
            case 'salvarQuantidadeDestinada':
                $this->salvarQuantidadeDestinada($_POST);
                break;
            case 'modalEscolas':
                $this->modalEscolas($_POST['ipiid'], $_POST['eciid'], 1, $_POST['muncod']);
                break;
            case 'atualizarListaModalEscolas':
                $this->modalEscolas($_POST['ipiid'], $_POST['eciid'], $_POST['pagina']);
                break;
            case 'salvarQuantidadeEscola':
                $this->salvarQuantidadeEscola($_POST);
                break;
            case 'salvarArrayItem':
                $this->salvarArrayItem($_POST);
                break;
            case 'listaEscolasTermo':
                $dados['proid'] = $proid;
                $dados['inuid'] = (int) $_GET['inuid'];
                $dados['muncod'] = $_POST['muncod'];
                $dados['ipiid'] = $_POST['ipiid'];
                $dados['escno_localizacao'] = $_POST['escno_localizacao'];
                $dados['esccodinep'] = $_POST['esccodinep'];
                $dados['mostrarTodas'] = $_POST['mostrarTodas'];
                $dados['ineid'] = $dadosProcesso['ineid'];
                $this->listaEscolasTermo($dados, $_POST['pagina']);
                break;
        }
    }

    /**
     * @param $proid - par3.documentotermo
     * @return string
     */
    public function recuperarProcesso($proid)
    {
        return $this->model->recuperarProcesso($proid);
    }

    public function listaContratos($proid)
    {

        $sql = $this->model->sqlListaContratos($proid);

        $cabecalho = array('Número do Contrato', 'Fornecedor', 'Razão Social', 'Data do Contrato', 'QTD Itens', 'Valor do Contrato', 'Saldo', 'Valor pago', 'Situação da última solicitação', 'Status contrato');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas('ecostatus', 'arqid', 'dotdatafimvigencia');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addAcao('edit', 'editarContrato');
        $listagem->addAcao('download', ['func' => 'downloadArquivoContrato', 'titulo' => 'Download Contrato', 'extra-params' => ['arqid'], 'cor' => 'success']);
        $listagem->addAcao('pagamento', ['func' => 'modalSolicitacaoDesembolsoContrato', 'titulo' => 'Solicitar Desembolso', 'extra-params' => ['ecovalor', 'dotdatafimvigencia'], 'cor' => 'warning']);
        $listagem->addAcao('remove', ['func' => 'removerContrato', 'outline' => 'btn-outline']);
        $listagem->setAcaoComoCondicional('remove', [['campo' => 'ecostatus', 'op' => 'igual', 'valor' => 'A']]);
        $listagem->addCallbackDeCampo(['ecovalor', 'saldo'], 'par3_mascaraMoeda');
        $listagem->addCallbackDeCampo('ecocnpj', 'formatar_cnpj');
        $listagem->addCallbackDeCampo('totalpago', 'formataNumeroMonetarioComSimbolo');
        $listagem->turnOffForm();
        $listagem->setIdLinha('linhaContrato');;
        $listagem->turnOnPesquisator();
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    /**
     * Lista as iniciativas do termo
     * @param $processo
     * @return array|mixed|NULL
     */
    public function recuperaIniciativas($processo)
    {
        return $this->model->recuperaIniciativas($processo);
    }

    private function montaOpcaoIncluirTodos($inpid)
    {
        echo "<div class='col-lg-2' style='padding-left: 0;'>
                <button class='btn btn-default incluirtodos' type='button' data-inpid='{$inpid}'  onclick='incluirtodos(this)'>Incluir todos os itens</button>
              </div>";
    }

    /**
     * Lista os itens da iniciativa selecionada
     * @param $inpid
     */
    public function listaItens($inpid)
    {

        $this->montaOpcaoIncluirTodos($inpid);

        $idcontrato = $_SESSION['idcontrato'];
        $sql = $this->model->sqlListaItens($inpid, $idcontrato);

        $cabecalho = array('Descrição do Item', 'Ano', 'Contratos do Item', 'Qtd', 'Qtd sem contrato', 'Valor', 'Valor Total', 'Quantidade', 'Valor', 'Quantidade recebida de bens e serviços');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->escondercolunas('aicaicid', 'eciaicid', 'statuscontrato', 'condicaoedicao', 'aicvaloraprovado', 'eciid');
        $listagem->turnOnPesquisator();
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo(['valorreferencia','valortotal'], 'par3_mascaraMoeda');
        $listagem->addCallbackDeCampo('contratos', 'campoButtonIcon');
        $listagem->addCallbackDeCampo('inputqtd', 'Par3_Model_ExecucaoContrato::inputQtd');
        $listagem->turnOffForm();
        $listagem->render();

        if ($idcontrato) {
            echo '<script>formatarValorItem();</script>';
        }
    }

    public function salvarContrato($dados)
    {

        $cnpj = pega_numero($dados['ecocnpj']);
        $razaosocial = ($dados['ecorazaosocialfornecedor']);
        $cpfresponsavel = pega_numero($dados['ecocpfresponsavel']);
        $numero = $dados['econumero'];
        $data = $dados['ecodta'];
        $desc = ($dados['ecodsc']);
        $qtd = $dados['ecoqtd'];
        $valor = $dados['ecovalor'];

        $campos = ['ecocnpj' => $cnpj,
            'ecorazaosocialfornecedor' => $razaosocial,
            'ecocpfresponsavel' => $cpfresponsavel,
            'econumero' => $numero,
            'ecodta' => $data,
            'ecodsc' => $desc,
            'ecoqtd' => $qtd,
            'ecovalor' => $valor,
            'proid' => $dados['proid'],
            'usucpfinclusao' => $_SESSION['usucpf']];

        //editar
        if ($dados['ecoid'] > 0) {
            /*if (!$this->model->verificaValorContrato($dados['ecoid'], $dados['ecovalor'])) {
                echo 'valorcontrato';
                exit;
            }*/

            $campos['ecoid'] = $dados['ecoid'];
            if (count($_FILES['file']['type']) > 0) {
                if ($_FILES['file']['type'] != 'application/pdf') {
                    echo 'filetype';
                    exit;
                }

                $file = new FilesSimec('execucao_contrato', null, 'par3');
                if ($dados['arqid'] != '') {
                    $file->excluiArquivoFisico($dados['arqid']);
                }

                $file->setUpload('PAR3 - Plano de Trabalho - Execução e Acompanhamento - Contrato', null, false);
                $campos['arqid'] = $file->getIdArquivo();
                $this->model->popularDadosObjeto($campos);
                $idContrato = $this->model->salvar();
                $result = $this->model->commit();
            } else {
                $this->model->popularDadosObjeto($campos);
                $idContrato= $this->model->salvar();
                $result = $this->model->commit();
            }
        } else { //incluir
            if ($_FILES['file']['type'] != 'application/pdf') {
                echo 'filetype';
                exit();
            }

            $_FILES['file']['name'] = ($_FILES['file']['name']); // Necessário devido ao 'charset:false' do ajax, que é necessário para mandar o arquivo no próprio ajax
            $file = new FilesSimec('execucao_contrato', null, 'par3');
            $file->setUpload('PAR3 - Plano de Trabalho - Execução e Acompanhamento - Contrato', null, false);
            $campos['arqid'] = $file->getIdArquivo();
            $this->model->popularDadosObjeto($campos);
            $idContrato = $this->model->salvar();
            $result = $this->model->commit();
        }
        $_SESSION['idcontrato'] = $idContrato;

        if ($result) {
            echo $idContrato;
        } else {
            echo 'falha';
        }
    }

    public function removerContrato($ecoid)
    {

        if (!$this->model->verificaPodeRemoverContrato($ecoid)) {
            echo 'existedesembolso';
            die;
        }

        $ecocnpj = $this->model->executar('SELECT ecocnpj FROM par3.execucao_contrato WHERE ecoid = ' . $ecoid);

        $this->model->ecoid = $ecoid;
        $this->model->ecostatus = 'I';
        $this->model->ecocpfinativacao = $_SESSION['usucpf'];
        $this->model->ecodtinativacao = date('Y-m-d H:i:s', time());

        try {
            $this->model->salvar();
            $this->model->commit();
            echo $ecocnpj;
        } catch (Simec_Db_Exception $e) {
            echo 'falha';
        }
    }

    public function salvarItem($dados)
    {

        $id = $this->model->salvarItem($dados['aicid'], $dados['ecoid'], $dados['eciqtditens'], $dados['ecivaloritens'], $dados['eciid']);

        if(is_array($id)){
            echo simec_json_encode($id);
        }else if ($id > 0 || $id == '') {
            $result['eciid'] = $id;
            $result['total'] = $this->model->recuperarValorTotalItens($dados['ecoid'])[0];
            echo simec_json_encode($result);
        } else {
            echo 'falha';
        }
    }

    public function salvarArrayItem($dados)
    {
        $this->model->inserirArrayItem($dados['item']);
        $result = $this->model->recuperarValorTotalItens($dados['item'][0]['ecoid']);
        echo simec_json_encode($result);
    }

    public function editarContrato($dados)
    {
        //id setado na sessão para que a lista de itens seja atualizada via ajax ao editar o contrato
        $_SESSION['idcontrato'] = $dados['ecoid'];

        $result = $this->model->dadosEditarContrato($dados['ecoid']);

        echo simec_json_encode($result);
    }

    /**
     * Lista os contratos do item selecionado (botão na listagem)
     * @param $aicid
     */
    public function listaContratosItem($aicid)
    {
        $sql = $this->model->sqlContratosItem($aicid);

        $cabecalho = array('Número do Contrato', 'Quantidade item', "Valor");

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addAcao('edit', 'editarContratoModal');
        $listagem->addCallbackDeCampo('ecivaloritens', 'par3_mascaraMoeda');
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    /* * * * MONITORAMENTO * * * */

    public function salvarQuantidadeDestinada($dados)
    {
        echo $this->model->salvarQuantidadeDestinada($dados['eciid'], $dados['eciqtddestinada']);
    }

    public function salvarQuantidadeEscola($dados)
    {
        $result = $this->model->salvarQuantidadeEscola($dados['eciid'], $dados['escid'], $dados['emeqtd'], $dados['emeid'], $dados['ipeid']);
        echo $result;
    }

    /**
     * Tive que criar aqui uma espécie de paginação, colocando limit no sql para diminuir o peso das requisições.
     * O componente do simec listagem não estava mostrando todas as escolas quando o número passava de 200,
     * e como a página está trabalhando só com ajax, a paginação do próprio Listagem também não serviu (pois ele recarrega a página).
     * @param $aicid
     * @param $eciid
     * @param int $paginaAtual
     * @param int $postMuncod
     */
    public function modalEscolas($ipiid, $eciid, $paginaAtual = 1, $postMuncod = null)
    {

        //offset para paginação
        $itensPorPagina = 70;

        if (!$_SESSION['idcontrato']) {
            echo 'idcontrato';
            die;
        }

        echo "<input type='hidden' id='modalescolasipiid' value=\"{$ipiid}\">
              <input type='hidden' id='modalescolaseciid' value=\"{$eciid}\">";

        $sql = "SELECT estuf, muncod, itrid FROM par3.instrumentounidade WHERE inuid = {$_GET['inuid']}";
        $dadosUnidade = $this->model->pegaLinha($sql);

        if($dadosUnidade['itrid'] == 1){

            global $simec;
            $modelMunicipio = new Par3_Model_Municipio();
            $sqlMunicipiosUF = $modelMunicipio->montaSQLComboMunicipiosUF($_GET['inuid']);
            echo '<div class="ibox-content">
                    <div class="col-lg-offset-3">';
            echo $simec->select('escmuncod', '', $postMuncod, $sqlMunicipiosUF, array('maxlength' => '255','data-placeholder' => 'Selecione o Município'), array('input-size' => '8'));
            echo '</div></div>';


            $sql = $this->model->sqlModalEscolas($eciid, $ipiid, $_SESSION['idcontrato'], $dadosUnidade, $postMuncod);
            $escolas = $this->model->carregar($sql);

            if(isset($postMuncod)){
                echo '<br><b><span style="width:20px; height:20px; border-left: 20px solid #f0bec3;"></span>&nbsp; Escolas Extintas</b>';
                $this->listaModalEscolas($escolas);
            }

        }else {

            $sql = $this->model->sqlModalEscolas($eciid, $ipiid, $_SESSION['idcontrato'], $dadosUnidade);
            $escolas = $this->model->carregar($sql);
            $this->listaModalEscolas($escolas);
        }

    }

    public function listaModalEscolas($escolas){
        $cabecalho = array(
            'Município',
            'Código INEP',
            'Escola',
            'Localização',
            'Esfera',
            'Quantidade',
            'Quantidade destinada',
            'Quantidade de salas',
            'Quantidade de alunos'
        );

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setDados($escolas);
        $listagem->esconderColunas('totalescolas', 'escco_situacao_funcoinamento');
        $listagem->turnOffForm();
        $listagem->turnOnPesquisator();
        $listagem->setTamanhoPagina(1000);
        $listagem->addRegraDeLinha(array('campo' => 'escco_situacao_funcoinamento', 'op' => 'igual', 'valor' => [3,4], 'classe' => 'linha_vermelha'));
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render();
    }
    /* * * * FIM MONITORAMENTO * * * */


    public function listaEscolasTermo($dados, $paginaAtual = 1, $itensPorPagina = 100)
    {

        if($dados['muncod'] || $dados['ipiid'] || $dados['escno_localizacao'] || $dados['esccodinep']){
            $dados['mostrarTodas'] = true;
        }

        $offset = ($paginaAtual -1) * $itensPorPagina;
        $sql = $this->model->sqlEscolasTermo($dados, $offset, $itensPorPagina);

        $arrEscolas = $this->model->carregar($sql);

        if(!empty($arrEscolas) > 0){
            echo '<button class="btn btn-primary" style="position: absolute;" type="button" id="gerarexcel" onclick="gerarExcelEscolasTermo()"><i class="fa fa-file-excel-o"></i> Gerar Excel</button>';
        }

        echo '<div>';

        $cabecalho = array('Município', 'Código INEP', 'Código Planejamento', 'Escola', 'Etapa', 'Localização', 'Esfera', 'Item', 'Quantidade', 'Quantidade destinada', 'Quantidade de salas', 'Quantidade de alunos');

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setDados($arrEscolas);
        $listagem->esconderColunas('totalescolas', 'muncod');
        $listagem->turnOffForm();
        $listagem->turnOnPesquisator();

        if($dados['mostrarTodas']){
            $listagem->setTamanhoPagina(99999);
            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        }

        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

        if(!$dados['mostrarTodas']) {
            $totalEscolas = $this->model->retornaTotalEscolas($dados['proid'], $dados['ineid']);

            $this->montaHtmlPaginacaoLista($totalEscolas, $itensPorPagina, $paginaAtual, 'atualizarListaEscolasTermo');
        }
        echo '</div>';
    }

    private function montaHtmlPaginacaoLista($total, $itensPorPagina, $paginaAtual, $requisicao)
    {
        //--- Paginação da lista
        //total de itens detalhados
        $totalEscolas = $total;
        //detalhados x total
        if ($totalEscolas > 0) {
            echo "<span style='float:right'><b>Total de escolas: {$totalEscolas}</b></span><div style=\"clear: both;\"></div>";
        }

        //Html do número de páginas
        if ($totalEscolas > $itensPorPagina) {
            $quantidadePaginas = intval($totalEscolas / $itensPorPagina) + ($totalEscolas%$itensPorPagina > 0 ? 1 : 0);
            $anteriorDiabled = '';
            $funcaoAnterior = 'onclick="'.$requisicao.'(' . ($paginaAtual-1) . ')"';

            if ($paginaAtual < 2) {
                $anteriorDiabled = 'disabled';
                $funcaoAnterior = '';
            }

            $htmlPaginacao = '<nav style="float: right">
                                  <ul class="pagination">
                                    <li class="page-item '.$anteriorDiabled.'">
                                      <a class="page-link" '.$funcaoAnterior.'>Anterior</a>
                                    </li>'; //atualizarListaDetalhamento(pagina)

            for ($i = 1; $i <= $quantidadePaginas; $i++) {
                $active = '';
                $funcaoPAtual = 'onclick="'.$requisicao.'(' . $i . ')"';
                if ($i == $paginaAtual) {
                    $active = ' active ';
                    $funcaoPAtual = '';
                }
                $htmlPaginacao .= '<li class="page-item ' . $active . '"><a class="page-link" '.$funcaoPAtual.'>'. $i .'</a></li>';
            }

            $funcaoProximo = 'onclick="'.$requisicao.'(' . ($paginaAtual + 1) . ')"';
            $proximoDisabled = '';
            if ($paginaAtual == $quantidadePaginas) {
                $funcaoProximo = '';
                $proximoDisabled = 'disabled';
            }

            $htmlPaginacao .= '<li class="page-item '.$proximoDisabled.'">
                                  <a class="page-link" '.$funcaoProximo.'>Próximo</a>
                                </li>
                              </ul>
                            </nav><div style="clear: both;"></div>';

            echo $htmlPaginacao;
        }
    }

    public function gerarExcelEscolasTermo($proid, $inuid)
    {
        $dados['proid'] = $proid;
        $dados['inuid'] = $inuid;
        $sql = $this->model->sqlEscolasTermo($dados, 0, 0, true);
        $cabecalho = $cabecalho = array('Município', 'Código INEP', 'Escola', 'Localização', 'Esfera', 'Item', 'Quantidade', 'Quantidade destinada', 'Quantidade de salas', 'Quantidade de alunos');
        $this->model->sql_to_excel($sql, 'lista_escolas_termo_'.$proid, $cabecalho, 's', false);
    }
}
