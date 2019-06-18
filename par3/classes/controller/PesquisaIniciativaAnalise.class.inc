<?php
 
class Par3_Controller_PesquisaIniciativaAnalise extends Controle
{
    public function __construct()
    {
        //$desid = validarInteiro($_REQUEST['desid'])? $_REQUEST['desid']:'';
        $this->model = new Par3_Model_PesquisaIniciativaAnalise();
        //$this->modelHistorico = new Par3_Model_IniciativaDesdobramentosHistorico();
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
            $sql = $this->model->montarSQLSimples($arrPost);
            
            if(count($sql) > 0){
                $listagemSimec = new Simec_Listagem();
                $arrayCabecalho = array('Código','Dimensão','UF', 'Entidade', 'Iniciativa','Programa','Tipo de Objeto','Etapa', 'Modalidade','Desdobramentos','Valor','Situação', 'Valor Aprovado');
                $esconderColunas = array('qtd','inuid', 'iniid', 'nivel','anos','ciclo','tipoatendimento','areas', 'projetos','cronoinicio','cronofim','cronoano','descricao');
                $listagemSimec->setCabecalho($arrayCabecalho);
                $listagemSimec->setFormFiltros('form-iniciativaplanejamento');
                $listagemSimec->esconderColunas($esconderColunas);
                $listagemSimec->turnOnPesquisator();
                $listagemSimec->setQuery($sql);
                //$listagemSimec->addAcao('plus', 'detalharAnalise' );
                $listagemSimec->addAcao('plus', array('func' => 'detalharAnalise', 'extra-params' => array('iniid')) );
                $listagemSimec->addCallbackDeCampo('situacao', 'formata_status');
                $listagemSimec->addCallbackDeCampo('valor', 'formataNumeroMonetarioComSimbolo');
                $listagemSimec->addCallbackDeCampo('total_aprovado', 'formataNumeroMonetarioComSimbolo');
                $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
                $listagemSimec->setCampos($arrayCabecalho);
                $listagemSimec->setTamanhoPagina(30);

                $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
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
            SELECT distinct ip.inpid, a.anaid, ip.inuid, ip.iniid, iia.iniano, coalesce(es.esddsc, 'Não Iniciado') as esddsc, coalesce(u.usunome, '-') as usunome,
            	coalesce(to_char(a.anadatacriacao, 'DD/MM/YYYY'), '-') as dataanalise, a.anaid as assistencia, p.pronumeroprocesso, va.vlr_aprovado
            FROM par3.iniciativa_planejamento ip
            	inner join par3.iniciativa_iniciativas_anos iia on iia.iniid = ip.iniid
            	inner join par3.iniciativa_itenscomposicao_grupo iicg on iicg.iniid = iia.iniid
            	left join par3.iniciativa_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipiano = iia.iniano and ipc.ipistatus = 'A'
            	left join par3.analise a
            		inner join workflow.documento d on d.docid = a.docid
            		inner join workflow.estadodocumento es on es.esdid = d.esdid
            		left join seguranca.usuario u on u.usucpf = a.cpftecnico
                    LEFT JOIN par3.iniciativa_tipos_assistencia it ON it.intaid = a.intaid
            	on a.inpid = ip.inpid and a.anaano = iia.iniano and a.anastatus = 'A'
            	LEFT JOIN(
            		SELECT anaid, sum(vlr_aprovado) AS vlr_aprovado FROM(
            			SELECT ai.anaid, (ai.aicqtdaprovado * ai.aicvaloraprovado) AS vlr_aprovado FROM par3.analise_itemcomposicao ai WHERE ai.aicstatus = 'A' AND ai.aicqtdaprovado > 0
            		) AS foo GROUP BY anaid
            	) va ON va.anaid = a.anaid
            	LEFT JOIN par3.processoparcomposicao ppc
            		INNER JOIN par3.processo p ON p.proid = ppc.proid AND p.prostatus = 'A'
            	ON ppc.anaid = a.anaid AND ppc.inpid = ip.inpid AND ppc.ppcstatus = 'A'
            WHERE ipc.ipistatus = 'A'
            	and ip.iniid = {$iniid}
            	and ip.inpid = {$inpid}
            order by iia.iniano
DML;

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        $tratamentoListaVazia = ($arrPost['req'] == '' ) ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;
        $cabecalho = array('Ano','Situação','Técnico','Data de Análise', 'Tipo Assistência', 'Processo', 'Valor Aprovado');
        $esconderColunas = array('iniid', 'inuid', 'anaid');
        $listagem = new Simec_Listagem(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        $listagem->setCabecalho($cabecalho);
        $listagem->turnOnPesquisator();
        $listagem->setQuery($sql);
        $listagem->esconderColunas($esconderColunas);
        $listagem->addAcao('edit', array('func' => 'CadastrarAnalise', 'extra-params' => array('inuid', 'iniano', 'anaid')) );
        $listagem->addCallbackDeCampo('assistencia', 'popOverEmendas');
        $listagem->addCallbackDeCampo('vlr_aprovado', 'formataNumeroMonetarioSemSimbolo');
        $listagem->addCallbackDeCampo('pronumeroprocesso','formata_numero_processo');
        $listagem->turnOnOrdenacao();

        $listagem->render($tratamentoListaVazia);
        
        echo '<script>$(function () {
                  $(\'[data-toggle="popover"]\').popover()
                })</script>';
    }
}