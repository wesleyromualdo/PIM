<?php
/**
 * Classe de controle do  Relatório Geral do PAR
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   THIAGO TASCA BARBOSA <thiagotasca@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 15-10-2018
 * @link     no link
 */

 /**
 * Par3_Controller_RelatorioGeralPar
 *
 * @category Class
 * @package  A1
 * @author    THIAGO TASCA BARBOSA <thiagotasca@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 15-10-2018
 * @link     no link
 */
class Par3_Controller_RelatorioGeralPar extends Controle
{
    private $model;
    public  $colEntidadeSolicitadas;

    public function __construct()
    {
        $this->model = new Par3_Model_RelatorioGeralPAR();
    }

    private function dadosSelecionados($arrPost){

    }

    private function validacaoDeDados(){

    }


    public function carregaRelatorioGeralPar($arrPost = array())
    {   

        $arrayColunasSql            = array();
        $arrayColunasGroupBySql     = array();
        $arrayCabecalho             = array();
        $arrayColunasSqlWith        = array();
        $filtros                    = '';
        $filtrosInterno             = '';
        $filtrosiniciativadados     = '';
        $listagemSimec              = new Simec_Listagem();
        $anoControleFiltro          = 0;
        $tipoTabelasSelecionadas    = array();
        $tabelasprioritarias        = array();
        $objetoFiltro               = false;
        $filtroIniciativa           = false;

        // CONTROLA MONTAGEM DO SQL
        $solicitadoItem             = false;
        $solicitadoProcesso         = false;

        // CONTROLE PARA O FILTRO DE ANO. VERIFICA QUAL ANO UTILIZAR COM O TIPO DE PESQUISA. 
        $anoProcesso                = false;
        $anoItem                    = false;
        $anoEmpenho                 = false;
        $agrupaEmpenho              = false;

        // REGRAS GERAIS DO RELATÓRIO
        if ( (!array_key_exists("planejamentoentidade", $arrPost)) &&
            (!array_key_exists("colunasentidades", $arrPost)) && 
            (!array_key_exists("analisefnde", $arrPost)) && 
            (!array_key_exists("colunasprocessos", $arrPost)) && 
            (!array_key_exists("colunasempenhos", $arrPost))
            ) {
            alert('É necessário selecionar pelo menos uma coluna');
            return false;
        }

        //TRAVA DE OBRAS COM OUTROS ITENS
        $obras = false;
        $colunas = null;  
        $colunas =  explode(",", $arrPost['selctordemcolunas']);
        array_pop($colunas);

        $QtdPlanejamentoEnt = count( $arrPost['planejamentoentidade']);
        if( $QtdPlanejamentoEnt > 0 ){
            if( in_array("nomeobras", $arrPost['planejamentoentidade']) ){
                $obras = true;
            }
            if( in_array("idobra", $arrPost['planejamentoentidade']) ){
                $obras = true;
            }
            if( $obras ){
                foreach ($arrPost['planejamentoentidade'] as $key => $value) {
                    if($value != 'idobra' && $value != 'nomeobras' ){
                        alert('Não é possível efetuar uma busca referente a lista de obras junto com dados de itens, iniciativas e análise.');
                        return false;
                    }
                }
            }
        }

        //ANALISES DE POSSIBILIDADES MONTADAS PELO USUARIO
        if( in_array("vlrempprocesso", $colunas) && in_array("ne", $colunas)){
            $agrupaEmpenho = true; 
        }  
        if( in_array("p.anoitem", $colunas) ){
            $anoItem = true; 
        }  
        if( in_array("it.item", $colunas) ){
            $solicitadoItem = true;
        } 
        if( in_array("id.inddsc", $colunas) ){
            $iniciativa = true; 
        } 
        if( in_array("id.iniid", $colunas) ){
            $iniciativa = true; 
        }
        
        
        // VARRENDO COLUNAS SELECIONADAS
        foreach ($colunas as $key => $coluna) {
            //COLUNAS ENTIADE
            if($coluna == 'coluf'){
                array_push($arrayCabecalho, 'UF' );
                array_push($arrayColunasSql, 'iu.estuf' );
                array_push($arrayColunasGroupBySql, 'iu.estuf');
            }
            if($coluna == 'colmuncod'){
                array_push($arrayCabecalho, 'IBGE' );
                array_push($arrayColunasSql, 'iu.muncod' );
                array_push($arrayColunasGroupBySql, 'iu.muncod');
            }
            if($coluna == 'colentidade'){
                array_push($arrayCabecalho, 'Entidade' );
                array_push($arrayColunasSql, 'iu.inudescricao' );
                array_push($arrayColunasGroupBySql, 'iu.inudescricao');
            }
            if($coluna == 'colesddsc'){
                array_push($arrayCabecalho, 'Fase do PAR' );
                array_push($arrayColunasSql, 'ed.esddsc' );
                array_push($arrayColunasGroupBySql, 'ed.esddsc');
                if( !in_array("colfasepar", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colfasepar' ); 
                }
            }

            // COLUNAS PLANEJAMENTO ENTIDADE
            if($coluna == 'id.tipoobjeto'){
                array_push($arrayCabecalho, 'Objeto' );
                array_push($arrayColunasSql, 'p.tipoobjeto' );
                array_push($arrayColunasGroupBySql, 'p.tipoobjeto');
                if( !in_array("iniciativaDados", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'iniciativaDados' ); 
                }
                if( !in_array("planejamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'planejamento' ); 
                }
            }
            if($coluna == 'id.inddsc'){
                array_push($arrayCabecalho, 'Iniciativa' );
                array_push($arrayColunasSql, 'p.inddsc' ); 
                array_push($arrayColunasSqlWith, 'inddsc' );
                array_push($arrayColunasGroupBySql, 'p.inddsc');
                if( !in_array("iniciativaDados", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'iniciativaDados' ); 
                }
                if( !in_array("planejamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'planejamento' ); 
                }
            }
            if($coluna == 'it.item'){
                array_push($arrayCabecalho, 'Itens' );
                array_push($arrayColunasSql, 'p.item' );
                array_push($arrayColunasSqlWith, 'item' );
                array_push($arrayColunasGroupBySql, 'p.item');
                if( !in_array("planejamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'planejamento' ); 
                }
            }
            if($coluna == 'moddsc'){
                array_push($arrayCabecalho, 'Modalidade' );
                array_push($arrayColunasSql, 'p.moddsc' );
                array_push($arrayColunasGroupBySql, 'p.moddsc');
                if( !in_array("planejamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'planejamento' ); 
                }
            }
            if($coluna == 'idobra'){
                array_push($arrayCabecalho, 'ID Obra' );
                array_push($arrayColunasSql, 'o.obrid' );
                array_push($arrayColunasGroupBySql, 'o.obrid');
                if( !in_array("obras", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'obras' ); 
                }
                
            }
            if($coluna == 'nomeobras'){
                array_push($arrayCabecalho, 'Nome da Obra' );
                array_push($arrayColunasSql, 'o.obrdsc' );
                array_push($arrayColunasGroupBySql, 'o.obrdsc');
                if( !in_array("obras", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'obras' ); 
                }
            }
            if($coluna == 'p.anoitem'){
                array_push($arrayCabecalho, 'Ano Item' );
                array_push($arrayColunasSql, 'p.anoitem' );
                array_push($arrayColunasGroupBySql, 'p.anoitem');
                if( !in_array("planejamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'planejamento' ); 
                }
                $anoControleFiltro += 1;
                $solicitadoItem = true;
            } 
            if($coluna == 'p.qtditem'){
                array_push($arrayCabecalho, 'QTD Itens Solicitado' ); 
                array_push($arrayColunasSqlWith, 'qtditem' );
                if( $iniciativa && !$solicitadoItem  ){
                    array_push($arrayColunasSql, 'sum(p.qtditem)' );
                } else if ($anoItem) {
                    array_push($arrayColunasSql, 'p.qtditem' );
                    array_push($arrayColunasGroupBySql, 'p.qtditem');
                    $solicitadoItem = true;
                }else{  
                    array_push($arrayColunasSql, 'sum(p.qtditem)' );
                    $solicitadoItem = true;
                }
                if( !in_array("planejamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'planejamento' ); 
                }
            }
            if($coluna == 'p.valoritem'){
                array_push($arrayColunasSqlWith, 'valoritem' );
                
                if( $iniciativa && !$solicitadoItem  ){
                    array_push($arrayCabecalho, 'Valor Referência' );
                    array_push($arrayColunasSql, 'sum(p.valoritem) AS valoritem' );
                }
                else{
                    array_push($arrayCabecalho, 'Valor Referência' );
                    array_push($arrayColunasSql, 'p.valoritem AS valoritem' );
                    array_push($arrayColunasGroupBySql, 'p.valoritem');
                }
                
                $listagemSimec->addCallbackDeCampo(['valoritem'], 'mascaraReal');
                $solicitadoItem = true;
                
                if( !in_array("planejamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'planejamento' ); 
                }
            }
            if($coluna == 'vlrXqtdItem'){
                array_push($arrayCabecalho, 'Valor Total Solicitado (QTD X Valor)' );
                $listagemSimec->addCallbackDeCampo(['valortotalitemsol'], 'mascaraReal');
                
                if(  !in_array("valoritem", $arrayColunasSqlWith) ){
                    array_push($arrayColunasSqlWith, 'valoritem' );
                }
                if(  !in_array("qtditem", $arrayColunasSqlWith) ){
                    array_push($arrayColunasSqlWith, 'qtditem' );
                }
                if( $iniciativa && !$solicitadoItem  ){
                    array_push($arrayColunasSql, ' sum(p.valoritem) as valortotalitemsol ' );
                }else{
                    array_push($arrayColunasSql, ' (sum(p.qtditem) * p.valoritem) as valortotalitemsol ' );
                    array_push($arrayColunasGroupBySql, 'p.valoritem');
                }
                $solicitadoItem = true;
                if( !in_array("planejamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'planejamento' ); 
                }
            }

            if($coluna == 'id.cronograma'){
                array_push($arrayCabecalho, 'Cronograma' );
                array_push($arrayColunasSql, 'p.cronograma' );
                array_push($arrayColunasGroupBySql, 'p.cronograma');
                if( !in_array("iniciativaDados", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'iniciativaDados' ); 
                }
                array_push($tabelasprioritarias, 'planejamento' );
            }
            if($coluna == 'id.tipoassistencia'){
                array_push($arrayCabecalho, 'Tipo assistencia' );
                array_push($arrayColunasSql, 'p.tipoassistencia' );
                array_push($arrayColunasGroupBySql, 'p.tipoassistencia');
                if( !in_array("iniciativaDados", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'iniciativaDados' ); 
                }
                if( !in_array("planejamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'planejamento' );
                }
            }
            if($coluna == 'au.autnome'){
                array_push($arrayCabecalho, 'Autor da Emenda' );
                array_push($arrayColunasSql, 'au.autnome' );
                array_push($arrayColunasGroupBySql, 'au.autnome');
                if( !in_array("emenda", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'emenda' );
                }
                if( !in_array("planejamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'planejamento' );
                }
                if( !in_array("colanalise", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colanalise' );
                }
            }
            if($coluna == 'p.inpid'){
                array_push($arrayCabecalho, 'Cód do Planejamento' );
                array_push($arrayColunasSql, 'p.inpid' );
                array_push($arrayColunasGroupBySql, 'p.inpid');
                if( !in_array("planejamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'planejamento' ); 
                }
            }
            if($coluna == 'id.iniid'){
                array_push($arrayCabecalho, 'Cód Iniciativa' );
                array_push($arrayColunasSql, 'p.iniid' );
                array_push($arrayColunasGroupBySql, 'p.iniid');
                if( !in_array("iniciativaDados", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'iniciativaDados' ); 
                }
                if( !in_array("planejamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'planejamento' ); 
                }
            }

            //COLUNAS ANALISE
            if($coluna == 'p.ipiquantidadeaprovada'){
                array_push($arrayCabecalho, 'Quantidade Aprovada' );
                $solicitadoItem = true;
             
                if(is_array($arrPost['planejamentoentidade']) ){
                    if (in_array("p.anoitem", $arrPost['planejamentoentidade'])) {
                        array_push($arrayColunasSql, 'coalesce(p.aicqtdaprovado,0) AS qtdaprovada' );
                        array_push($arrayColunasGroupBySql, 'qtdaprovada');
                    }else{
                        array_push($arrayColunasSql, 'sum(coalesce(p.aicqtdaprovado,0)) AS qtdaprovada' );
                    }
                }else{
                    array_push($arrayColunasSql, 'sum(coalesce(p.aicqtdaprovado,0)) AS qtdaprovada' );
                }
               
                if( !in_array("planejamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'planejamento' ); 
                }
                if( !in_array("colanalise", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colanalise' ); 
                }
            }
            if($coluna == 'valoraprovadoitem'){
                array_push($arrayCabecalho, 'Valor Aprovado Item' );
                array_push($arrayColunasSql, 'sum(p.valoritem * p.aicqtdaprovado) AS valor_total_aprovado' );
                $listagemSimec->addCallbackDeCampo(['valor_total_aprovado'], 'mascaraReal');
                //array_push($arrayColunasGroupBySql, 'valor_total_aprovado');
                $solicitadoItem = true;   
                if( !in_array("planejamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'planejamento' ); 
                }
                if( !in_array("colanalise", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colanalise' ); 
                }
                if(  !in_array("valoritem", $arrayColunasSqlWith) ){
                    array_push($arrayColunasSqlWith, 'valoritem' );
                }
            } 
            if($coluna == 'p.itemaprovado'){
                if (!in_array("p.anoitem", $arrPost['planejamentoentidade'])) {
                    alert('Para utilizar a coluna item aprovado é necessário colocar a coluna Ano item.');
                    return false;
                }
                array_push($arrayCabecalho, 'Situação do Item' );
                array_push($arrayColunasSql, " CASE WHEN p.aicqtdaprovado > 0 THEN 'Sim' ELSE 'Não' END AS itemaprovado "  );
                array_push($arrayColunasGroupBySql, 'itemaprovado');
                $solicitadoItem = true;
                if( !in_array("colanalise", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colanalise' ); 
                }
            }
            if($coluna == 'codanalise'){
                array_push($arrayCabecalho, 'Cód da Análise' );
                array_push($arrayColunasSql, 'p.anaid' );
                array_push($arrayColunasGroupBySql, 'p.anaid');
                if( !in_array("colanalise", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colanalise' ); 
                }
            } 
            if($coluna == 'situacaoanalise'){
                array_push($arrayCabecalho, 'Situação da Análise' );
                array_push($arrayColunasSql, 'p.esddscanalise' );
                array_push($arrayColunasGroupBySql, 'p.esddscanalise');
                if( !in_array("colFaseAnalise", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colFaseAnalise' );
                }
                if( !in_array("colanalise", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colanalise' ); 
                }
            }

            // COLUNAS PROCESSOS
            if($coluna == 'numprocesso'){
                array_push($arrayCabecalho, 'Número do Processo' );
                array_push($arrayColunasSql, ' formata_num_processo(pro.pronumeroprocesso) as pronumeroprocesso ' );
                array_push($arrayColunasGroupBySql, 'pro.pronumeroprocesso');
                $solicitadoProcesso = true;
                //23400.002154/2017-85
                
            }
            if($coluna == 'tipoprocesso'){
                array_push($arrayCabecalho, 'Tipo de Processo' );
                array_push($arrayColunasSql, " case when ( select count(proid) from par3.processoobracomposicao where pocstatus = 'A' and proid = pro.proid ) > 0 then 'Obras' else 'PAR' end AS protipo " );
                array_push($arrayColunasGroupBySql, 'protipo');
                array_push($arrayColunasGroupBySql, 'pro.proid');
                $solicitadoProcesso = true;
                array_push($tabelasprioritarias, 'tipoprocesso' );
                
            }
            if($coluna == 'anoprocesso'){
                array_push($arrayCabecalho, 'Ano do processo' );
                array_push($arrayColunasSql, 'substr(pronumeroprocesso, 12, 4) as anoprocesso' );
                array_push($arrayColunasGroupBySql, 'anoprocesso');
                $anoControleFiltro                += 1;
                $anoProcesso    = true;
                $solicitadoProcesso = true;
            }
            if($coluna == 'valorprocesso'){ // VER A PARTE DE OBRAS COMO FAZER O VALOR
                array_push($arrayCabecalho, 'Valor do Processo' );
                array_push($arrayColunasSql, ' vsaldo.vlr_termo AS valorprocesso ' );
                $listagemSimec->addCallbackDeCampo(['valorprocesso'], 'mascaraReal');
                array_push($arrayColunasGroupBySql, 'valorprocesso');
                array_push($tabelasprioritarias, 'valorprocesso' );
                $solicitadoProcesso = true;
            }
            if($coluna == 'tipoobjeto'){
                array_push($arrayCabecalho, 'Tipo Objeto' );
                array_push($arrayColunasSql, " array_to_string(array(SELECT DISTINCT t.intodsc FROM par3.iniciativa_tipos_objeto t
                INNER JOIN(
                    SELECT intoid, proid FROM par3.processoparcomposicao WHERE ppcstatus = 'A'
                    UNION ALL
                    SELECT intoid, proid FROM par3.processoobracomposicao WHERE pocstatus = 'A'
                ) pp ON pp.intoid = t.intoid
                WHERE t.intostatus = 'A'
                AND pp.proid = pro.proid), ', ') AS tipoobjeto " );
                array_push($arrayColunasGroupBySql, 'tipoobjeto');
                
                $solicitadoProcesso = true;
                array_push($tabelasprioritarias, 'tipoobjeto' );
                array_push($arrayColunasGroupBySql, 'pro.proid');
                
            }
 
            //EMPENHOS 
            if($coluna == 'ne'){
                if($agrupaEmpenho){
                    array_push($arrayCabecalho, 'NE' );
                    array_push($arrayColunasSql, ' array_agg(empenho.empnumero) ' );
                    array_push($arrayColunasGroupBySql, 'empenho.empnumero');
                }else{
                    array_push($arrayCabecalho, 'NE' );
                    array_push($arrayColunasSql, 'empenho.empnumero' );
                    array_push($arrayColunasGroupBySql, 'empenho.empnumero');
                }
                if( !in_array("colEmpenhos", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colEmpenhos' );
                }
            }
            if($coluna == 'anoempenho'){
                array_push($arrayCabecalho, 'Ano Empenho' );
                array_push($arrayColunasSql, 'empenho.empanooriginal' );
                array_push($arrayColunasGroupBySql, 'empenho.empanooriginal');
                $anoControleFiltro                 += 1;
                $anoEmpenho = true;
                if( !in_array("colEmpenhos", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colEmpenhos' );
                }
            }
            
            if($coluna == 'valorempenho'){
                array_push($arrayCabecalho, 'Valor Empenho' );
                array_push($arrayColunasSql, 'empenho.valorempenhado' );
                $listagemSimec->addCallbackDeCampo(['valorempenhado'], 'mascaraReal');
                array_push($arrayColunasGroupBySql, 'empenho.valorempenhado');
                if( !in_array("colEmpenhos", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colEmpenhos' );
                }
            }
            if($coluna == 'situacaoempenho'){
                array_push($arrayCabecalho, 'Situação do Empenho' );
                array_push($arrayColunasSql, 'empenho.empsituacao' );
                array_push($arrayColunasGroupBySql, 'empenho.empsituacao');
                if( !in_array("colEmpenhos", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colEmpenhos' );
                }
        
            }
            if($coluna == 'ptres'){
                array_push($arrayCabecalho, 'PTRES' );
                array_push($arrayColunasSql, 'empenho.empcodigoptres' );
                array_push($arrayColunasGroupBySql, 'empenho.empcodigoptres');
                if( !in_array("colEmpenhos", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colEmpenhos' );
                }
        
            }
            if($coluna == 'fonte'){
                array_push($arrayCabecalho, 'Fonte' );
                array_push($arrayColunasSql, 'empenho.empfonterecurso' );
                array_push($arrayColunasGroupBySql, 'empenho.empfonterecurso');
                if( !in_array("colEmpenhos", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colEmpenhos' );
                }
                
            } 
            if($coluna == 'vlrempprocesso'){ // VENDO REGRA DE AGRUPAMENTO DE EMPENHO VARIAVEL REGRA NA MONTA LISTA. 
                array_push($arrayCabecalho, 'Valor Total Empenhado no Processo' );
                array_push($arrayColunasSql, 'empenho.valorempenhado as totalempproc' );
                $listagemSimec->addCallbackDeCampo(['totalempproc'], 'mascaraReal');
                if( !in_array("colEmpenhos", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colEmpenhos' );
                }
            } 
            

            // PAGAMENTOS
            if($coluna == 'ob'){
                array_push($arrayCabecalho, 'OB' );
                array_push($arrayColunasSql, 'pag.pagnumeroob' );
                array_push($arrayColunasGroupBySql, 'pag.pagnumeroob');
                if( !in_array("colPagamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colPagamento' );
                }
            }
            if($coluna == 'numseqpagamento'){
                array_push($arrayCabecalho, 'Num Seg Pagamento' );
                array_push($arrayColunasSql, 'pag.pagnumseqob' );
                array_push($arrayColunasGroupBySql, 'pag.pagnumseqob');
                if( !in_array("colPagamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colPagamento' );
                }
            }
            if($coluna == 'anoefetivacaopagamento'){
                array_push($arrayCabecalho, 'Ano Efetivacao Pagmento' );
                array_push($arrayColunasSql, 'pag.paganoparcela' );
                array_push($arrayColunasGroupBySql, 'pag.paganoparcela');
                if( !in_array("colPagamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colPagamento' );
                }
            }
            if($coluna == 'vlrpago'){
                array_push($arrayCabecalho, 'Valor Pago' );
                array_push($arrayColunasSql, 'pag.pagvalorparcela' );
                $listagemSimec->addCallbackDeCampo(['pagvalorparcela'], 'mascaraReal');
                array_push($arrayColunasGroupBySql, 'pag.pagvalorparcela');
                if( !in_array("colPagamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colPagamento' );
                }
            }
            if($coluna == 'sitpagamento'){
                array_push($arrayCabecalho, 'Situacao do Pagamento' );
                array_push($arrayColunasSql, 'pag.pagsituacaopagamento' );
                array_push($arrayColunasGroupBySql, 'pag.pagsituacaopagamento');
                if( !in_array("colPagamento", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colPagamento' );
                }
            }

            //COLUNAS TERMOS
            if($coluna == 'numtermo'){
                array_push($arrayCabecalho, 'Termo' );
                array_push($arrayColunasSql, 'dot.dotnumero||\'-\'||dot.intoid as termo' );
                array_push($arrayColunasGroupBySql, 'termo');
                if( !in_array("colTermos", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colTermos' );
                }
            }
            if($coluna == 'tipotermo'){
                array_push($arrayCabecalho, 'Tipo Termo' );
                array_push($arrayColunasSql, 'ito.intodsc' );
                array_push($arrayColunasGroupBySql, 'ito.intodsc');
                if( !in_array("colTermos", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colTermos' );
                }
            }
            if($coluna == 'valortermo'){
                array_push($arrayCabecalho, 'Valor Termo' );
                array_push($arrayColunasSql, 'coalesce(dot.dotvalortermo,0) as valor_termo' );
                $listagemSimec->addCallbackDeCampo(['valor_termo'], 'mascaraReal');
                array_push($arrayColunasGroupBySql, 'valor_termo');
                if( !in_array("colTermos", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colTermos' );
                }
                
            }
            if($coluna == 'valorfnde'){
                array_push($arrayCabecalho, 'Valor FNDE' );
                array_push($arrayColunasSql, '( coalesce(dot.dotvalortermo,0) - coalesce(dot.dotvalorcontrapartida,0) ) as valorfnde' );
                $listagemSimec->addCallbackDeCampo(['valorfnde'], 'mascaraReal');
                array_push($arrayColunasGroupBySql, 'valorfnde');
                if( !in_array("colTermos", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colTermos' );
                }
                
            }
            if($coluna == 'valorcontrapartida'){
                array_push($arrayCabecalho, 'Valor Contrapartida' );
                array_push($arrayColunasSql, 'dot.dotvalorcontrapartida as contrapartida' );
                $listagemSimec->addCallbackDeCampo(['contrapartida'], 'mascaraReal');
                array_push($arrayColunasGroupBySql, 'contrapartida');
                if( !in_array("colTermos", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'colTermos' );
                }
                
            }
        
        }

        // ANALISE DE FILTROS SELECIONADOS PELO USUARIO

        $inuid = ' ';
        $arrFiltros = array();
        $todasEsferas = false;

        //FILTROS E CONTROLES DE JOINS PARA O SQL
       // ver($arrPost); die();
        foreach ($arrPost as $key => $value) {
                    
            // FAZER CONTROLES DE $tipoTabelasSelecionadas
            // ------------------- CONTROLES --------------------
            if($key == 'colunasentidades'){   
                $colunaSelecionadasEntidades =  count($value);
                if( $colunaSelecionadasEntidades > 0){
                    array_push($tabelasprioritarias, 'entidade' );
                } 
            }
            if( $key == 'planejamentoentidade'){
                $colunaSelecionadasPlanejamento =  count($value);
                if( $colunaSelecionadasPlanejamento > 0){
                    array_push($tabelasprioritarias, 'planejamento' );
                }
            }
            if($key == 'analisefnde'){  
                $colunaSelecionadasAnalise =  count($value);
                if( $colunaSelecionadasAnalise > 0){
                    array_push($tabelasprioritarias, 'analise' );
                }
            }
            if($key == 'colunasprocessos'){ 
                $colunaSelecionadasProcessos =  count($value);
                if( $colunaSelecionadasProcessos > 0){
                    array_push($tabelasprioritarias, 'processos' );
                }
            }
            if($key == 'colunasempenhos'){ 
                $colunaSelecionadasEmpenhos =  count($value);
                if( $colunaSelecionadasEmpenhos > 0){
                    array_push($tabelasprioritarias, 'empenho' );
                    $solicitadoProcesso = true;
                }
            }
            if( $key == 'colunafinanceiro'){
                $colunaSelecionadasFinanceiro =  count($value);
                if( $colunaSelecionadasFinanceiro > 0){
                    array_push($tabelasprioritarias, 'financeiro' );
                    $solicitadoProcesso = true;
                }
            }
            if($key == 'colunastermos'){
                $colunaSelecionadasTermos =  count($value);
                if( $colunaSelecionadasTermos > 0){
                    array_push($tabelasprioritarias, 'colTermos' );
                }
            }
            
            //-------------------- FILTROS --------------------//

            // Esfera
            if($key == 'itrid'){
                if($value != '-1'){
                    $itridSQL = ' AND iu.itrid = '.$value.' ';
                    $itrid = $value;
                    $filtros .= $itridSQL;
                }else{
                    $todasEsferas = true;
                }
            }

            // Estados
            if($key == 'estuf'){
                if($value){
                    if($itrid == 1 ){ // estados
                        $estados = implode("','",$value);
                        $filtrosEstuf = "AND iu.estuf in ('".$estados."')";
                        $arrinuid = $this->model->recuperaInuid($filtrosEstuf, $itrid);
                       
                        
                    }else if ($itrid == 2){ // municipios
                        $estados = implode("','",$value);
                        $filtrosEstuf = " AND iu.estuf in ('".$estados."')";
                        $arrinuid = $this->model->recuperaInuid($filtrosEstuf, $itrid);
                        
                    }else{ // todos
                        $estados = implode("','",$value);
                        $filtrosEstuf = " AND iu.estuf in ('".$estados."')";
                        $arrinuid = $this->model->recuperaInuid($filtrosEstuf, 3);
                    }
                    $arrayInuid2 = array();
                    foreach ($arrinuid as $value) {
                        array_push($arrayInuid2, $value['inuid'] );
                    }
                    $inuid .= implode("','",$arrayInuid2);
                }
            }

            // Municípios
            if($key == 'muncod'){
                
                if($value){
                    $municipios     = implode("','",$value);
                    $filtrosMuncod   = " AND iu.muncod in ('".$municipios."')";
                    $arrinuid       = $this->model->recuperaInuid($filtrosMuncod, 2);
                    $arrayInuid2    = array();
                    foreach ($arrinuid as $value) {
                        array_push($arrayInuid2, $value['inuid'] );
                    }
                    
                    if($inuid != " " && ( $todasEsferas || $itrid == 1 ) ){
                        $inuid2  = implode("','",$arrayInuid2);
                        $inuid =  $inuid."','".$inuid2;
             
                    }else{
                        $inuid  = implode("','",$arrayInuid2);
                    }
                   
                }
            }
            //ver($inuid,d);
            // fases da analise
            if($key == 'esdidAnalise'){
                if( $value ){
                    if( !in_array("colanalise", $tabelasprioritarias) ){
                        alert('Não é possível filtrar por Situação de análise sem nenhuma coluna de análise selecionada.');
                        return false;
                    }
                    $esdidAnalise     = $value;
                    $filtros    .= " AND p.esdid in ('". $esdidAnalise."')";
                    if($esdidAnalise == '2070' ){
                        array_push($tabelasprioritarias, 'filtrofaseAguardAnalise' );
                    }
                    array_push($tabelasprioritarias, 'filtrofaseAnalise' );
                    //array_push($tabelasprioritarias, 'filtroitemaprovado' );
                }
            }
            
            // fases do par
            if($key == 'esdidFiltros'){
                
                if($value !=''){
                    //ver($value); die();
                    $esdid   = $value;
                    $filtros        .= " AND ed.esdid ='".$esdid."' ";
                    if( !in_array("colfasepar", $tabelasprioritarias) ){
                        array_push($tabelasprioritarias, 'colfasepar' ); 
                    }
                }
            }

            // processos
            if($key == 'pronumeroprocesso'){
                if($value){
                    $processo   = $value;
                    $arrinuid   = $this->model->recuperaInuid($processo, '0', 'processo');
                    $arrayInuid2 = array();
                    foreach ($arrinuid as $value) {
                        array_push($arrayInuid2, $value['inuid'] );
                    }
                    $inuid          = implode("','",$arrayInuid2);
                    $filtros        .= " AND pro.pronumeroprocesso ='".$processo."' ";
                   // $filtrosInterno .= " AND ip.inuid in ('". $inuid."')";
                   if( !in_array("processo", $tabelasprioritarias) ){
                    array_push($tabelasprioritarias, 'processo' );
                   }
                    $solicitadoProcesso = true;
                }
            } 

            // Filtro N Empenho
            if($key == 'filtroNE'){
                if($value){
                    $numNE   = $value;
                    $arrinuid   = $this->model->recuperaInuid($numNE, '0', 'empenho');
                    $arrayInuid2 = array();
                    foreach ($arrinuid as $value) {
                        array_push($arrayInuid2, $value['inuid'] );
                    }
                    $inuid          = implode("','",$arrayInuid2);
                    $arrFiltros['filtroNE'] = $numNE;
                    if( !in_array("processo", $tabelasprioritarias) ){
                        array_push($tabelasprioritarias, 'processo' );
                    }
                    if( !in_array("empenho", $tabelasprioritarias) ){
                        array_push($tabelasprioritarias, 'empenho' );
                    }
                    $solicitadoProcesso = true;
                }
            } 

            // Termo
            if($key == 'dotid'){
                if($value){
                    
                    $termo   = implode("','",$value);
                    $arrinuid   = $this->model->recuperaInuid($termo, '0', 'termo');
                
                    $arrayInuid2 = array();
                    foreach ($arrinuid as $value) {
                        array_push($arrayInuid2, $value['inuid'] );
                    }
                    $inuid          = implode("','",$arrayInuid2);
                    //dot.dotnumero||\'-\'||dot.intoid
                    $filtros        .= " AND dot.dotid ='".$termo."' ";
                    //$filtrosInterno .= " AND ip.inuid in ('". $inuid."') ";
                    array_push($tabelasprioritarias, 'termo' );
                    array_push($tabelasprioritarias, 'filtroTermo' );
                
                }

            }
            
            // itens aprovados
            if($key == 'itemaprovado'){
                if($value == 1){
                   
                    if( !in_array("colanalise", $tabelasprioritarias) ){
                        alert('Não é possível filtrar por itens aprovados sem nenhuma coluna de análise selecionada.');
                        return false;
                    }
                   $itemaprovado = true;
                    $arrFiltros['filtroItemAprovado'] = " AND coalesce(aicqtdaprovado,0) > 0   ";
                    array_push($tabelasprioritarias, 'filtroitemaprovado' );
                
                }else if($value == 2){
                    $arrFiltros['filtroItemAprovado'] = " AND coalesce(aicqtdaprovado,0) = 0  ";
                    array_push($tabelasprioritarias, 'filtroitemnaoaprovado' );
                }

            }

            // itens com Adesao
            if($key == 'itemadesao'){
                if($value == 1){
                   
                    if( !$solicitadoProcesso || !in_array('planejamento', $tabelasprioritarias)  ){ //|| !in_array("colanalise", $tabelasprioritarias)
                        alert('Não é possível filtrar por itens com adesão sem nenhuma coluna de Planejmaento e processo selecionada.');
                        return false;
                    }
                    array_push($tabelasprioritarias, 'filtroitemaadesaoSim' );
                
                }else if($value == 2){
                    array_push($tabelasprioritarias, 'filtroitemaadesaoNao' );
                }

            }

            if($key == 'tipoobjetoFiltro'){
                if(count($value) >= 1 ){
                    $tipooOjetoFiltro            = implode("','",$value);
                    $filtros                    .= " AND p.intoid in( '".$tipooOjetoFiltro."') ";
                    $filtrosiniciativadados     .= " AND i.intoid in( '".$tipooOjetoFiltro."') ";
                    $objetoFiltro  = true;          
                }
            }

            if($key == 'tipoassistencia'){
                if($value > 0){
                    if(  !in_array('planejamento', $tabelasprioritarias)  ){ //|| !in_array("colanalise", $tabelasprioritarias)
                        alert('Não é possível filtrar por Tipo de assistência sem nenhuma coluna de iniciativa do campo Planejmaento.');
                        return false;
                    }

                    if(!in_array('processo', $tabelasprioritarias)){
                        array_push($tabelasprioritarias, 'processo' );
                    }
                    $arrFiltros['filtroAtendimento'] = " AND tipas.intaid = ".$value." ";
                }
            }

            // iniciativas
            if($key == 'iniciativaFiltro'){
                //ver($value); die();
                if(count($value) >= 1 ){
                    $tipoIniciativaFiltro            = implode("','",$value);
                    $filtros                    .= " AND p.iniid in('".$tipoIniciativaFiltro."') ";
                    $filtrosiniciativadados     .= " AND i.iniid in('".$tipoIniciativaFiltro."') ";
                    $filtroIniciativa = true;                  
                }
            }

            // itens
            if($key == 'itemFiltros'){
                if(count($value) >= 1 ){
                    $itemFiltro            = implode("','",$value);
                    $filtros              .= " AND p.itcid in(".$itemFiltro.") "; 
                    $solicitadoItem        = true; 
                    array_push($tabelasprioritarias, 'item' );              
                }
            }
            
            // emendas
           if($key == 'emendafiltro'){
                if($value){
                    if(!in_array('planejamento', $tabelasprioritarias) ){
                        alert("Não é possivel utilizar o filtro de Emendas sem ter selecionado alguma coluna de Emendas.");
                        return false;
                    }
                    $arrFiltros['filtroEmenda'] = " AND ie.emeid in(".$value.") ";
                    if( !in_array("emenda", $tabelasprioritarias) ){
                        array_push($tabelasprioritarias, 'emenda' );
                    }               
                }
            } 

            // ID OBRA
            if($key == 'obridfiltro'){
                if($value){
                    if(!in_array('planejamento', $tabelasprioritarias) ){
                        alert("Não é possivel utilizar o filtro de Código de Obra sem ter selecionado alguma coluna de obras no item Planejamento.");
                        return false;
                    }
                    $arrFiltros['filtroobra'] = " AND poc.obrid in(".$value.") ";
                    if( !in_array("obras", $tabelasprioritarias) ){
                        array_push($tabelasprioritarias, 'obras' );
                    } 
                }
            }

            // Cod da Iniciativa
            if($key == 'codplafiltro'){
                if($value){
                    if(!in_array('planejamento', $tabelasprioritarias) ){
                        alert("Não é possivel utilizar o filtro de Iniciativas sem ter selecionado alguma coluna do Planejamento.");
                        return false;
                    }
                    $arrFiltros['codplafiltro'] = $value;
                    if( !in_array("codplafiltro", $tabelasprioritarias) ){
                        array_push($tabelasprioritarias, 'codplafiltro' );
                        $filtroIniciativa = true;
                    } 
                }
            }
           
            // Anos itens
            if($key == 'ano'){
                if($value){
                    $ano   = $value;
                    if($anoControleFiltro > 1 ){
                        alert("Não é possivel efetuar o filtro de Ano com mais de uma coluna de ano selecionada. Escolha apenas uma coluna de ano nas colunas.");
                        return false;
                    }
                    if($anoProcesso){
                        $filtros .= " AND  substr(pronumeroprocesso, 12, 4) = '".$ano."' ";
                        array_push($tabelasprioritarias, 'processo' );
                    }
                    if($anoItem){
                        $filtros .= " AND p.anoitem = '".$ano."' ";
                        $filtrosInterno .=  " AND ipc.ipiano = '".$ano."' ";
                        array_push($tabelasprioritarias, 'item' );
                    }
                    if($anoEmpenho){
                        $filtros .= " AND empenho.empanooriginal = '".$ano."' ";
                        array_push($tabelasprioritarias, 'empenho' );
                    }
                }
            }

            // Situacao do Termo
            if($key == 'situacaoTermo'){
                switch ($value){
                    case '0':
                        // Quer todos
                        break;
                    case '1':
                        // Pendente
                        if(!in_array('termo', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'termo' );
                        }
                        if(!in_array('termoPendente', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'termoPendente' );
                        }
                        break;
                    case '2':
                        // Validado
                        if(!in_array('termo', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'termo' );
                        }
                        if(!in_array('termoValidado', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'termoValidado' );
                        }
                        break;
                    case '3':
                        // Rejeitado
                        if(!in_array('termo', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'termo' );
                        }
                        if(!in_array('termoRejeitado', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'termoRejeitado' );
                        }
                        break;
                }
            }

            // Termos com Contrato
            if($key == 'termoComContato'){
                switch ($value){
                    case '0':
                        // Quer todos
                        break;
                    case '1':
                        // Termos com Contrato
                        if(!in_array('termo', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'termo' );
                        }
                        if(!in_array('termoComContrato', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'termoComContrato' );
                        }
                        break;
                    case '2':
                        // Termos sem Contrato
                        if(!in_array('termo', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'termo' );
                        }
                        if(!in_array('termoSemContrato', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'termoSemContrato' );
                        }
                        break;
                }
            }

            // Termos com Notas
            if($key == 'TermoComNota'){
                switch ($value){
                    case '0':
                        // Quer todos
                        break;
                    case '1':
                        // Termos com Notas
                        if(!in_array('termo', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'termo' );
                        }
                        if(!in_array('TermoComNota', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'TermoComNota' );
                        }
                        break;
                    case '2':
                        // Termos sem Notas
                        if(!in_array('termo', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'termo' );
                        }
                        if(!in_array('TermoSemNota', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'TermoSemNota' );
                        }
                        break;
                }
            }

            
            // Empenho
            if($key == 'situacaoempenho'){
                switch ($value){
                    case '-1':
                        // Nao selecionou nada
                        break;
                    case '0':
                        // Quer todos, tendo ou nao empenho
                        break;
                    case '1':
                        // Quer os com empenho total
                        $filtros .= " AND vsaldo.vlrempenho >= vsaldo.vlr_termo  ";
                        $solicitadoProcesso = true;
                        if(!in_array('empenho', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'empenho' );
                        }
                        if(!in_array('valorprocesso', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'valorprocesso' );
                        }
                        
                        break;
                    case '2':
                        // Quer os com empenho parcial
                        $filtros .= " AND vsaldo.vlr_termo > vsaldo.vlrempenho ";
                        $solicitadoProcesso = true;
                        if(!in_array('empenho', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'empenho' );
                        }
                        if(!in_array('valorprocesso', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'valorprocesso' );
                        }
                        break;
                    case '3':
                        // Quer os sem empenho
                        $filtros .= " AND vsaldo.vlrempenho is null ";
                        if(!in_array('empenho', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'empenho' );
                        }
                        if(!in_array('valorprocesso', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'valorprocesso' );
                        }
                        break;
                        
                }
            }
            
            // Pagamento
            if($key == 'situacaopagamento'){
                switch ($value){
                    case '-1':
                        // Não selecionou nada
                        break;
                    case '0':
                        // Quer todos, tendo ou nao pagamento
                        break;
                    case '1':
                        // Quer os com pagamento total
                        $filtros .= " AND vsaldo.vlrpago >= vsaldo.vlr_termo  ";
                        $solicitadoProcesso = true;
                        if(!in_array('pagamento', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'pagamento' );
                        }
                        if(!in_array('valorprocesso', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'valorprocesso' );
                        }
                        break;
                    case '2':
                        // Quer os com pagamento parcial
                        $filtros .= " AND vsaldo.vlr_termo > vsaldo.vlrpago ";
                        $solicitadoProcesso = true;
                        if(!in_array('pagamento', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'pagamento' );
                        }
                        if(!in_array('valorprocesso', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'valorprocesso' );
                        }
                        break;
                    case '3':
                        // Quer os sem pagamento
                        $filtros .= " AND vsaldo.vlrpago is null ";
                        if(!in_array('valorprocesso', $tabelasprioritarias)){
                            array_push($tabelasprioritarias, 'valorprocesso' );
                        }
                        break;
                }
            }

            
        } // fim do foreach - varre formulario
        // die();

        // VERIFICANDO CONDICOES FINAIS PARA MONTAR A QUERY
        
        if($inuid != ' '){
            $filtros .= " AND iu.inuid in ('". $inuid."')";
            $filtrosInterno .= " AND ip.inuid in ('". $inuid."')";
        }

        if($solicitadoItem){
            array_push($tabelasprioritarias, 'colunasitensselecionadas' );
        }
        if($solicitadoProcesso){
            array_push($tabelasprioritarias, 'colunasProcessoselecionadas' );
        }

        // TRAVAS NEGOCIAIS DO FORMULÁRIO
        if(!in_array('entidade', $tabelasprioritarias)){
            alert('É necessário selecionar pelo menos uma coluna de dados da Entidade. ');
            return false;
        }
        
        if (in_array('analise', $tabelasprioritarias) || in_array('planejamento', $tabelasprioritarias) ) { 
            if ( ( !array_key_exists("estuf", $arrPost) || !array_key_exists("muncod", $arrPost) ) && ( !$objetoFiltro || !$filtroIniciativa ) ) {
                $qtdMunicipio = count($arrPost['muncod']);
                $qtdEstados   = count($arrPost['estuf']);
                if( ($qtdMunicipio == 0) && ($qtdEstados == 0) ){
                    alert('É necessário selecionar pelo menos um estado ou município ou filtrar por algum objeto, iniciativa ou item para relatórios com dados de Planejamento e Análise');
                    return false;
                }
            }
        }

        if($arrPost['requisicao'] == 'exportar'){
            $sql = $this->model->sqlCarregaRelatorioGeralPar($arrayColunasSql, $arrayColunasGroupBySql, $filtros, $filtrosInterno, $tabelasprioritarias, $filtrosiniciativadados, $arrFiltros, $arrayColunasSqlWith );
            $list = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
            $list->setCabecalho($arrayCabecalho);
            $list->setQuery( $sql );
            $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        }

        // MONTA O RESULTADO - LISTA
        $sql = $this->model->sqlCarregaRelatorioGeralPar($arrayColunasSql, $arrayColunasGroupBySql, $filtros, $filtrosInterno, $tabelasprioritarias, $filtrosiniciativadados, $arrFiltros, $arrayColunasSqlWith );
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->setQuery($sql);
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setFormFiltros('formulario-relatorio');
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);

return true;
     
    }
}
?>