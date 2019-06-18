<?php

class Par3_Controller_VincularOrcamentaria extends Controle
{
    public function __construct()
    {
        
        //$this->model = new Par3_Model_VincularOrcamentaria();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function salvar($arrPost)
    {
        $acao = Par3_Model_IniciativaDesdobramentosHistorico::CREATE;
        if($arrPost['desid']){
            $arrayDesdobramento['desid'] = $arrPost['desid'];
            $acao = Par3_Model_IniciativaDesdobramentosHistorico::UPDATE;
        }
    }

    public function remover($arrPost)
    {
        
    }

    public function listar($arrPost){
           // $sql = $this->model->montarSQLSimples($arrPost);

                $sql = <<<DML
                SELECT ip.inpid, 
                       1000777 as codigoIniciativa, 
                       10002 as codigoAnalise, 
                       2017 as anoAnalise, 
                       'AC' as uf, 
                       'Acrelândia' as municipio, 
                       2017 as anoInici,
                       '31 - Obra - Construcao' as programa,
                       'PNE em movimento' as planoInterno,
                       'RFF08B90PRN' as ptres,
                       043952 as fonteRecurso,
                       10000000 as tpObjeto,
                       'FORMACAO' as situaIniciativa,
                       10007777 as dtUltimaAtualizacao
                FROM par3.iniciativa_planejamento ip
                   /* inner join par3.iniciativa_iniciativas_anos iia on iia.iniid = ip.iniid
                    inner join par3.iniciativa_itenscomposicao_grupo iicg on iicg.iniid = iia.iniid
                    inner join par3.iniciativa_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipiano = iia.iniano  
                    left join par3.analise a 
                        inner join workflow.documento d on d.docid = a.docid
                        inner join workflow.estadodocumento es on es.esdid = d.esdid
                        inner join seguranca.usuario u on u.usucpf = a.cpftecnico
                    on a.inpid = ip.inpid and a.anaano = iia.iniano and a.anastatus = 'A'
                WHERE ipc.ipistatus = 'A'
                order by iia.iniano       */
DML;
            
            if(count($sql) > 0){
                $listagemSimec = new Simec_Listagem();
                $arrayCabecalho = array('Código Iniciativa','Código da Análise','Ano da Análise','UF','Município','Ano da Iniciativa', 'Programa','Plano Interno','PTRES','Fonte de Recurso','Tipo de objeto','Situação da Iniciativa', 'Data da Última Atualização');
                $esconderColunas = array();
                $listagemSimec->setCabecalho($arrayCabecalho);
                $listagemSimec->esconderColunas($esconderColunas);
                $listagemSimec->turnOnPesquisator();
                $listagemSimec->setQuery($sql);
                //$listagemSimec->addAcao('plus', array('func' => 'detalharAnalise') );
               //$listagemSimec->addAcao('plus', array('func' => 'detalharAnalise', 'extra-params' => array('inpid')) );
                $listagemSimec->addCallbackDeCampo('situacao', 'formata_status');
                $listagemSimec->addCallbackDeCampo('valor', 'formataNumeroMonetarioComSimbolo');
                $listagemSimec->addAcao('remove', 'remover');
                $listagemSimec->addAcao('edit', 'vincularOrcamento');
                $listagemSimec->setTamanhoPagina(5);
                $listagemSimec->render();
            }else{
                $html = "<div class='row'>";
                $html .= "<div class='col-md-4'></div>";
                $html .= "<div class='col-md-4'><div class=\"alert alert-info\" role=\"alert\">Nenhum registro encontrado!</div></div>";
                $html .= "<div class='col-md-4'></div>";
                $html .= "</div>";
                echo $html;
            }
    }

    public function listarAnalises( $iniid, $inpid ){

        global $disabled;
        $sql = <<<DML
                SELECT distinct ip.inpid, ip.inuid, ip.iniid, iia.iniano, coalesce(es.esddsc, 'Não Iniciado') as esddsc, coalesce(u.usunome, '-') as usunome, coalesce(to_char(a.anadatacriacao, 'DD/MM/YYYY'), '-') as dataanalise
				FROM par3.iniciativa_planejamento ip
				    inner join par3.iniciativa_iniciativas_anos iia on iia.iniid = ip.iniid
				    inner join par3.iniciativa_itenscomposicao_grupo iicg on iicg.iniid = iia.iniid
				    inner join par3.iniciativa_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipiano = iia.iniano	
				    left join par3.analise a 
				    	inner join workflow.documento d on d.docid = a.docid
				        inner join workflow.estadodocumento es on es.esdid = d.esdid
				        inner join seguranca.usuario u on u.usucpf = a.cpftecnico
				    on a.inpid = ip.inpid and a.anaano = iia.iniano and a.anastatus = 'A'
				WHERE ipc.ipistatus = 'A'
					and ip.iniid = {$iniid}
					and ip.inpid = {$inpid}
				order by iia.iniano       
DML;

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        $tratamentoListaVazia = ($arrPost['req'] == '' ) ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;
        $cabecalho = array('Ano','Situação','Técnico','Data de Análise');
        $esconderColunas = array('iniid', 'inuid');
        $listagem = new Simec_Listagem(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        $listagem->setCabecalho($cabecalho);
        $listagem->turnOnPesquisator();
        $listagem->setQuery($sql);
        $listagem->esconderColunas($esconderColunas);
        $listagem->addAcao('edit', array('func' => 'CadastrarAnalise', 'extra-params' => array('inuid', 'iniano')) );
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }    
}