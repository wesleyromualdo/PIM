<?php

class Sac_Controller_Chamado
{
    /**
     * Função salvar
     * - retorna o evento da prefeitura.
     *
     * @return retorna um objeto evento prefeitura.
     *
     */
    public function salvar($dados)
    {
        global $url;

        $modelChamado = new Sac_Model_Chamado($dados['chaid']);
        $modelChamadoHistorico = new Sac_Model_ChamadoHistorico();
        $modelChamado->popularDadosObjeto($dados);

        try{
            if(!$modelChamado->chaid){
                $modelChamado->chadataabertura = date('Y-m-d H:i:s');
                $modelChamado->usucpfabertura = $_SESSION['usucpforigem'];
                $modelChamado->chastatus = Sac_Model_Chamado::K_STATUS_ABERTO;
            }

            $modelChamado->salvar(null, null, array('inuid', 'chaidpai'));

            if($modelChamado->chastatus == Sac_Model_Chamado::K_STATUS_ABERTO){
                $modelChamadoHistorico->chaid = $modelChamado->chaid;
                $modelChamadoHistorico->hicdata = date('Y-m-d H:i:s');
                $modelChamadoHistorico->usucpf = $_SESSION['usucpforigem'];
                $modelChamadoHistorico->hicdsc = $dados['chadsc'];
                $modelChamadoHistorico->chastatus = Sac_Model_Chamado::K_STATUS_ABERTO;
                $modelChamadoHistorico->salvar();

                $modelChamadoHistorico->adicionarAnexo();
            }

            $modelChamado->commit();

            simec_redirecionar($url . '&chaid=' . $modelChamado->chaid, 'success');
        } catch (Exception $e){
            $modelChamado->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function salvarHistorico($dados)
    {
        global $url;
        
        $modelChamadoHistorico = new Sac_Model_ChamadoHistorico();
        $modelChamadoHistorico->popularDadosObjeto($dados);
        try{
            $modelChamadoHistorico->hicdata = date('Y-m-d H:i:s');
            $modelChamadoHistorico->usucpf = $_SESSION['usucpforigem'];

            if(isset($dados['acao-historico'])){

                $modelChamado = new Sac_Model_Chamado($dados['chaid']);

                switch ($dados['acao-historico']){
                    case 'questionar-usuario':
                        $modelChamado->chastatus = $modelChamadoHistorico->chastatus = Sac_Model_Chamado::K_STATUS_AGUARDANDO_USUARIO;
                        break;
                    case 'responder-questionamento':
                        $modelChamado->chastatus = $modelChamadoHistorico->chastatus = Sac_Model_Chamado::K_STATUS_ABERTO;
                        break;
                    case 'enviar-validacao':
                        $modelChamado->chastatus = $modelChamadoHistorico->chastatus= Sac_Model_Chamado::K_STATUS_AGUARDANDO_VALIDACAO;
                        break;
                }
                $modelChamado->salvar();
            }

            $modelChamadoHistorico->salvar();
            $modelChamadoHistorico->adicionarAnexo();

            $modelChamadoHistorico->commit();

            simec_redirecionar($url . '&chaid=' . $modelChamadoHistorico->chaid, 'success');
        } catch (Exception $e){
            $modelChamadoHistorico->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function finalizar($dados)
    {
        global $url;

        $modelChamadoHistorico = new Sac_Model_ChamadoHistorico();
        $modelChamadoHistorico->popularDadosObjeto($dados);
        try{

            switch ($dados['finalizar']) {
                case Sac_Model_Chamado::K_STATUS_FINALIZADO_POSITIVO   : $descricao = 'Chamado Finalizado - Problema Solucionado';      break;
                case Sac_Model_Chamado::K_STATUS_FINALIZADO_NEGATIVO   : $descricao = 'Chamado Finalizado - Problema NÃO Solucionado';  break;
                case Sac_Model_Chamado::K_STATUS_FINALIZADO_AUTOMATICO : $descricao = 'Chamado Finalizado Automaticamente';             break;
                default: $descricao = '';
            }
            $modelChamadoHistorico->hicdata = date('Y-m-d H:i:s');
            $modelChamadoHistorico->usucpf = $_SESSION['usucpforigem'];
            $modelChamadoHistorico->hicdsc = $descricao;
            $modelChamadoHistorico->chastatus = $dados['finalizar'];
            $modelChamadoHistorico->salvar();

            $modelChamadoHistorico->adicionarAnexo();

            $modelChamado = new Sac_Model_Chamado($dados['chaid']);
            $modelChamado->chastatus = $dados['finalizar'];
            $modelChamado->salvar();

            $modelChamadoHistorico->commit();

            // Se o chamado for finalizado negativamente, abre possibilidade de abrir novo chamado vinculado
            if($dados['finalizar'] == Sac_Model_Chamado::K_STATUS_FINALIZADO_NEGATIVO){
                ob_clean();
                $url .= '&chaid=' . $modelChamadoHistorico->chaid . '&novo=1';
                header("Location: {$url}");
                die;
            } else {
                simec_redirecionar($url . '&chaid=' . $modelChamadoHistorico->chaid, 'success');
            }
        } catch (Exception $e){
            $modelChamadoHistorico->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function listarHistorico($chaid)
    {
        $modelChamadoHistorico = new Sac_Model_ChamadoHistorico();
        $sql = $modelChamadoHistorico->listarHistorico($chaid);

        $listagem = new Simec_Listagem();

        $cabecalho = array('Descrição', 'Data', 'CPF', 'Responsável');
        $listagem->setCabecalho($cabecalho);

        $listagem->addAcao('download', array('func' => 'baixarArquivo','extra-params' => array ('arqid')));
        $listagem->addAcao('view', 'exibirModalHistorico');

        $listagem->setAcaoComoCondicional('download',array(array('campo' => 'arqid', 'op' => 'diferente', 'valor' => -1)));
        $listagem->esconderColunas (array('arqid'));

        $listagem->setQuery($sql);


        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    public function listar($sisid = null, $filtros = array())
    {
        $sisid = $sisid ? $sisid : $_SESSION['sisid'];

        $modelChamado = new Sac_Model_Chamado();
        $sql = $modelChamado->listar($sisid, $filtros);

        $cabecalho = array('Código', 'Descrição', 'Situação', 'Data Abertura', 'Usuário Abertura', 'Data Situação', 'Usuário Situação', 'Tipo de Ocorrência', 'Onde ocorreu', 'Localização');

        if(!empty($filtros['gerar-excel'])) {
            ob_clean();
            $modelChamado->gerarExcel($sql, $cabecalho, 'chamados');
            exit;
        }

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);

//        $listagem->turnOnPesquisator();
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);

        $listagem->setQuery($sql);
        $listagem->addAcao('edit', 'exibirChamado');


        $listagem->turnOnOrdenacao();


        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    public function recuperarTipoPerfil($perfis = null)
    {
        $perfis = $perfis ? $perfis : pegaPerfilGeral();

        $modelPerfilTipo = new Sac_Model_PerfilTipo();
        return $modelPerfilTipo->recuperarTipoPerfil($perfis);
    }

    public function recuperarChamados($sisid, $filtros = array())
    {
        $sisid = $sisid ? $sisid : $_SESSION['sisid'];

        $modelChamado = new Sac_Model_Chamado();
        $sql = $modelChamado->listar($sisid, $filtros);
        $dados = $modelChamado->carregar($sql);
        return $dados ? $dados : array();
    }

    public function getSituacoes()
    {
        $modelChamado = new Sac_Model_Chamado();
        return $modelChamado->getSituacoes();
    }
}
?>