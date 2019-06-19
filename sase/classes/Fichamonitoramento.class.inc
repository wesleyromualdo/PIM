<?php

/**
 * Classe de mapeamento da entidade sase.fichamonitoramento.
 *
 * @version $Id$
 * @since 2016.08.12
 */

/**
 * Sase_Model_Fichamonitoramento: sem descricao
 *
 * @package Sase\Model
 * @uses Simec\Db\Modelo
 * @author Marilúcia Cardozo De Queiroz <marilucia.queiroz@mec.gov.br> e Victor Machado
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Sase_Model_Fichamonitoramento($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Sase_Model_Fichamonitoramento($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property \Datetime(Y-m-d H:i:s) $fmtdataalteracao 
 * @property \Datetime(Y-m-d H:i:s) $fmtdatainclusao 
 * @property string $usucpf 
 * @property string $fmtstatus  - default: 'A'::bpchar
 * @property string $fmtequipeatolegal 
 * @property string $fmtequipetecnica 
 * @property string $fmtcomnumanoatolegal 
 * @property string $fmtcomcoordenadora 
 * @property numeric $fmtperavalano3 
 * @property numeric $fmtperavalano2 
 * @property numeric $fmtperavalano1 
 * @property bool $fmtperavaltrianual 
 * @property bool $fmtperavalbianual 
 * @property bool $fmtperavalanual 
 * @property string $fmtpme 
 * @property string $muncod 
 * @property string $estuf 
 * @property int $fmtid  - default: nextval('sase.fichamonitoramento_fmtid_seq'::regclass)
 */

class Fichamonitoramento extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'sase.fichamonitoramento';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'fmtid',
    );


    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'fmtdataalteracao' => null,
        'fmtdatainclusao' => null,
        'usucpf' => null,
        'fmtstatus' => null,
        'fmtequipeatolegal' => null,
        'fmtequipecontato' => null,
        'fmtequipetelefone' => null,
        'fmtequipeemail' => null,
        'fmtequipetecnica' => null,
        'fmtcomnumanoatolegal' => null,
        'fmtcomcoordenadora' => null,
        'fmtperavalano4' => null,
        'fmtperavalano3' => null,
        'fmtperavalano2' => null,
        'fmtperavalano1' => null,
        'fmtperavalquadrienal' => null,
        'fmtperavalquinquenal' => null,
        'fmtperavalnaoprevisto' => null,
        'fmtperavaltrianual' => null,
        'fmtperavalbianual' => null,
        'fmtperavalanual' => null,
        'fmtpme' => null,
        'muncod' => null,
        'estuf' => null,
        'fmtid' => null,
    );


    /**
     * Campos da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arCampos = array(
        'fmtdataalteracao',
        'fmtdatainclusao',
        'usucpf',
        'fmtstatus',
        'fmtequipeatolegal',
        'fmtequipecontato',
        'fmtequipetelefone',
        'fmtequipeemail',
        'fmtequipetecnica',
        'fmtcomnumanoatolegal',
        'fmtcomcoordenadora',
        'fmtperavalano4',
        'fmtperavalano3',
        'fmtperavalano2',
        'fmtperavalano1',
        'fmtperavalquadrienal',
        'fmtperavalquinquenal',
        'fmtperavalnaoprevisto',
        'fmtperavaltrianual',
        'fmtperavalbianual',
        'fmtperavalanual',
        'fmtpme',
        'muncod',
        'estuf',
        'fmtid'
    );

	/**
	 * Atributos da Tabela obrigatórios
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosObrigatorios = array(
		'estuf',
		'muncod'
	);

	protected $atributosObrigatoriosString = array(
		'estuf' => 'Estado',
		'muncod' => 'Município'
	);

	/**
	 * Atributos Integer da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributosInt = array(
		'fmtid' => null,
		'fmtperavalano1' => null,
		'fmtperavalano2' => null,
		'fmtperavalano3' => null,
		'fmtperavalano4' => null
	);

	/**
	 * Array de lista de monitoramentos
	 *
	 * @var array
	 */
	public $lista;

	/**
	 * Id do último assessorado inserido
	 *
	 * @var integer
	 */
	public $inserido;


    public $arquivo;

    public function __construct(){
        $this->arquivo = new FilesSimec();
    }


    /**
     * Monta query para a função monta_lista da classe_simec.
     *
     * @param array $filtros
     * @author Sávio Resende
     */
    public function montaListaQuery( $filtros = false, $filtroSemPadrao = false, $order = false ){

        $acao  = "'<a style=\"cursor:pointer\" onclick=\"editarMonitoramento( ' || a.fmtid || ' )\"><img src=\"/imagens/alterar.gif\" title=\"Editar\"/></a>'";

        $indVerde = <<<HTML
    '<img title="Preenchido" src="/imagens/pnld/p_verde.gif"/>'
HTML;
        $indVermelho = <<<HTML
    '<img title="Não preenchido" src="/imagens/pnld/p_vermelho.gif"/>'
HTML;

        $sql = "
                SELECT
                    {$acao} as acao,
                    e.estdescricao as estado,
                    m.mundescricao as municipio,
                    CASE
                      WHEN
                            usucpf is not null 
                        AND fmtdatainclusao is not null 
                        AND fmtpme is not null
                        AND (fmtperavalanual  = 't' OR fmtperavalbianual = 't' OR fmtperavaltrianual = 't' OR fmtperavalquadrienal = 't' OR fmtperavalquinquenal = 't' OR fmtperavalnaoprevisto = 't')
                        AND fmtperavalano1 is not null
                        AND fmtcomcoordenadora is not null
                        AND fmtcomnumanoatolegal is not null
                        AND fmtequipetecnica is not null
                        AND fmtequipeatolegal is not null
                        AND ( fmtequipetelefone  is not null OR fmtequipeemail  is not null )
                      THEN {$indVerde}
                      ELSE {$indVermelho}
                    END as preenchido
                FROM " . $this->stNomeTabela . " a
                JOIN territorios.estado e ON e.estuf = a.estuf
                JOIN territorios.municipio m ON m.muncod = a.muncod";
        $sql .= " WHERE 1=1 AND fmtstatus = 'A'";
       // $sql .= " ORDER BY a.estuf, a.muncod ASC";

        if( $filtros != false ){
            foreach ($filtros as $chave => $valor)
                $sql .= " AND " . $chave . " = " . ((!in_array($chave, $this->arAtributosInt))?"'".$valor."'":$valor) . " ";
        }
        if( $filtroSemPadrao != false ){
            $sql .= " AND ";
            foreach ($filtroSemPadrao as $chave => $string)
                $sql .= " " . $string . " ";
        }
        if( $order != false){
            $sql.= " ORDER BY "."$order";
        }
        return $sql;
    }

        public function montaListaXls( $filtros = false, $filtroSemPadrao = false, $order = false ){
            global $db;


        $sql = "
                SELECT
                    e.estdescricao as estado,
                    m.mundescricao as municipio,
                    CASE
                      when usucpf is not null AND fmtdatainclusao is not null then 'Preenchido'
                      else 'Não preenchido'
                    END as preenchido
                FROM " . $this->stNomeTabela . " a
                JOIN territorios.estado e ON e.estuf = a.estuf
                JOIN territorios.municipio m ON m.muncod = a.muncod";
        $sql .= " WHERE 1=1 AND fmtstatus = 'A'";

        if( $filtros != false ){
            foreach ($filtros as $chave => $valor)
                $sql .= " AND " . $chave . " = " . ((!in_array($chave, $this->arAtributosInt))?"'".$valor."'":$valor) . " ";
        }
        if( $filtroSemPadrao != false ){
            $sql .= " AND ";
            foreach ($filtroSemPadrao as $chave => $string)
                $sql .= " " . $string . " ";
        }
        if( $order != false){
            $sql.= " ORDER BY "."$order";
        }
        
        //$res = $db->carregar($sql);
        $cabecalho = array("Estado","Município","Preenchido");
        $alinhamento = array('left','left','left');
        $larguras = array('35%','35%','30%');

        ob_clean();
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=simec_sase_fichatecnica_" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=simec_sase_fichatecnica_" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");

        $db->monta_lista($sql,$cabecalho,1000000,5,'N','','N','listaMonitoramento',$larguras,$alinhamento);
    }


    /**
     * Tratamento de regras de perfis para geração de filtros para 'montaListaQuery'
     * @param string $usucpf - CPF de login
     * @param string $sql - sql da lista
     * @param constante $tipo  - perfil DICOPE ou DIVAPE, se não declarar exibirá todos os estados ou municípios vinculados ao CPF.
     */
    public function trataRegrasPerfis( $usucpf, $sql ,$tipo = null ){

            $perfis = pegaPerfilGeral( $usucpf );
            
            $sql_temp = "
            	select 
					ur.pflcod AS pflcod,
					te.estuf AS estuf,
					ur.muncod AS muncod 
				from 
                 sase.usuarioresponsabilidade ur
                 Inner join territorios.municipio tm ON  tm.muncod = ur.muncod OR tm.estuf = ur.estuf
                 inner join territorios.estado te ON te.estuf = tm.estuf
                 where ur.usucpf = '{$usucpf}' AND ur.rpustatus = 'A'  ";
            $responsabilidades = $this->carregar( $sql_temp );
            
            // resgata municipios
            $municipios = array();
            if( !empty($responsabilidades) )
            foreach ($responsabilidades as $key => $value) {
            	if( !empty($value['muncod']) ){
                	$municipios[$value['pflcod']][] = $value['muncod'];
                    //array_push($municipios, $value['muncod']);
            	}
            }

            // resgata estados
            $estados = array();
            if( !empty($responsabilidades) )
            foreach ($responsabilidades as $key => $value) {
            	if( !empty($value['estuf']) ){
                	array_push($estados, $value['estuf']);
            	}
            }

            if( in_array( PFLCOD_SASE_TECNICO, $perfis)){
                    $sql .= " AND ( a.muncod in ('".(implode('\',\'',$municipios[PFLCOD_SASE_TECNICO]))."') )";
            }
            if( in_array( PFLCOD_SASE_SUPERVISOR_GERAL, $perfis)|| in_array( PFLCOD_SASE_EXECUTIVO,$perfis)){
                $sql .= " AND ( a.estuf in ('".(implode('\',\'',$estados))."') )";
            }

            return $sql;
    }



    /**
     * Valida dados por serem inseridos (campos obrigatorios)
     *
     * @param array dadosMonitoramento
     * @author Sávio Resende
     */
    public function validaDadosPorInserir( Array $dadosMonitoramento ){

            if( $this->verificaDuplicidade( $dadosMonitoramento ) ) return 'duplicidade';

            foreach ($this->arAtributosObrigatorios as $chave => $valor)
                    if( !isset($dadosMonitoramento[$valor]) || !$dadosMonitoramento[$valor] )
                            return $valor;

            return true;
    }



    /**
     * Verifica se já existe ficha cadastrada, para inativar caso exista.
     *
     * @param object $dadosMonitoramento
     * @author 
     */
    public function salvarHistorico( $dadosMonitoramento ){

        if( !isset($dadosMonitoramento->estuf) && !isset($dadosMonitoramento->muncod) ) return false;
        //verifica se existe registro para o município, se existir inativa todos. Na sequência inserirá novos dados fichaMonitoramento.inc.
        $existe = $this->pegaUm( " SELECT COUNT(*) as total FROM " . $this->stNomeTabela . " WHERE muncod = '" . $dadosMonitoramento->muncod . "' AND usucpf is not null" );
        if( $existe ){
            $sql = " UPDATE $this->stNomeTabela SET fmtstatus = 'I' WHERE muncod = '" . $dadosMonitoramento->muncod . "' ";
            $resultado = $this->executar( $sql );
            if( $resultado ) return $resultado;
        }
            return false;
    }

    /**
     * Exclui Logicamente Monitoramento
     *
     * @param integer $fmtid
     * @return bool
     * @author Sávio Resende
     */
    public function excluirLogicamenteMonitoramento( $fmtid ){
            $this->carregarPorId( $fmtid );
            $this->arAtributos['fmtstatus'] = 'I';
            $this->alterar();
            return $this->commit();
    }

    /**
     * Valida campos obrigatorios no objeto populado
     *
     * @author Sávio Resende
     * @return bool
     */
    public function validaCamposObrigatorios(){
            foreach ($this->arAtributosObrigatorios as $chave => $valor)
                    if( !isset($this->arAtributos[$valor]) || !$this->arAtributos[$valor] || empty($this->arAtributos[$valor]) )
                            return false;

            return true;
    }

    /**
     * Atualiza Monitoramento
     *
     * @return bool|string - retorna string 'invalido' caso existam campos obrigatorios vazios
     * @author Sávio Resende
     */
    public function atualizarMonitoramento(){
            if( $this->validaCamposObrigatorios() ){
                    $this->alterar();
                    return $this->commit();
            }

            return 'invalido';
    }

    /**
     * Popula Objeto com Array
     *
     * @param array $arDados
     * @return $this
     * @author Sávio Resende
     */
    public function populaMonitoramento( Array $arDados ){
            $this->popularObjeto( $this->arCampos, $arDados );
            return $this;
    }


    public function carregarPorMuncod($muncod){
    global $db;
    $sql = " select {$this->arChavePrimaria[0]} from {$this->stNomeTabela} where muncod = '{$muncod}'";
    $fmtid = $db->carregar($sql)[0][$this->arChavePrimaria[0]];
        if(!empty($fmtid)) {
            $this->carregarPorId($fmtid);
        }
    }

    public function dadosHistorico($municipio,$estado){
        global $db;
        $sql = "SELECT fmt.estuf,
                        mun.mundescricao,
                        fmtpme,
                        CASE 
                            WHEN fmtperavalanual = TRUE THEN 'Anual'
                            WHEN fmtperavalbianual = TRUE THEN 'Bianual'
                            WHEN fmtperavaltrianual = TRUE THEN 'Trianual'
                            WHEN fmtperavalquadrienal = TRUE THEN 'Quadrienal'
                            WHEN fmtperavalquinquenal = TRUE THEN 'Quinquenal'
                            WHEN fmtperavalnaoprevisto = TRUE THEN 'Não Previsto'
                            ELSE '<center> -- </center>'
                        END as periodo,
                        CASE 
                            WHEN fmtperavalano1 is not null THEN fmtperavalano1::varchar
                            ELSE '<center> -- </center>'
                        END as ano,
                        fmtcomcoordenadora,
                        fmtcomnumanoatolegal,
                        fmtequipetecnica,
                        fmtequipeatolegal,
                        CASE WHEN fmtstatus = 'I' THEN 'Inativo' ELSE 'Ativo' END fmtstatus,
                        fmt.usucpf ,
                        usunome,
                        fmtdatainclusao,
                        fmtdataalteracao 
                 FROM sase.fichamonitoramento fmt
                 JOIN seguranca.usuario usu ON usu.usucpf = fmt.usucpf
                 JOIN territorios.municipio mun ON mun.muncod = fmt.muncod
                 WHERE fmt.estuf = '{$estado}' AND fmt.muncod = '{$municipio}' AND fmtstatus = 'I'
                 ORDER BY fmtdatainclusao,fmtdataalteracao, fmt.usucpf ASC";
        $cabecalho = array("Estado","Município","Plano Municipal de Educação","Período","Ano(s)","Comissão", "Ato (n° e ano)", "Equipe", "Ato Legal","Status", "CPF","Nome","Inclusão","Alteração");
        $alinhamento = array('left','left','left','left','left','left','left','left','left');
        //$larguras = array('5%','10%','15%','5%','10%','15%','5%','10%','5%','1%','5%','8%','4%','4%');
        $historico = $db->monta_lista($sql,$cabecalho,30,5,'N','','N','historico',$larguras,$alinhamento); 
        return $historico;
    }
    /**
     * Monta legenda de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação
     *
     * @author Sávio Resende
     * @param array $estuf
     * @return html
     */
    public function montaLegenda( Array $estuf ){
        global $db;?>
        <div id="legendaMapaContainer">
        <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
            <ul>
                <?php
                require_once dirname(__FILE__) . '/Mapa/Metadados/DadoFichaMonitoramentoExterno.php';
                $classeDados = 'DadoFichaMonitoramentoExterno';
                $legenda = (new $classeDados())->dadosDaLegenda();
                
                foreach ($legenda as $value) {
                    echo <<<HTML
                        <li >
                           <table>
                              <tr>
                                 <td>
                                    <span style='background:{$value['cor']};' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;
                                        <b>{$value['total']}</b>&nbsp;&nbsp;
                                 </td>
                                 <td>
                                    {$value['descricao']}
                                 </td>
                              </tr>
                           </table>
                        </li>
HTML;
                }
                ?>
            </ul>
        </div><?php
    }
    /**
     * Monta legenda de acordo com os estados selecionados apresentando a quantidade de municipios envolvidos com a situação
     *
     * @author Sávio Resende
     * @param array $estuf
     * @return html
     */
    public function montaLegendaMunicipioExterno()
    {
        global $db; ?>
        <div id="legendaConteudo" style="display:block;float:right;width:285px;" class="topo_bootstrap_off">
            <div id="tituloLegenda">
                <div style="float:left;"><h5 style="font-size:14px !important;margin:2px !important;margin-left:0px !important;">&nbsp;Legenda:</h5></div>
                <div style="float:right;"></div>
                <div style="clear:both;height:1px;">&nbsp;</div>
            </div>
            <ul>
                <?php
                require_once dirname(__FILE__) . '/Mapa/Metadados/DadoFichaMonitoramentoExterno.php';
                $classeDados = 'DadoFichaMonitoramentoExterno';
                $legenda = (new $classeDados())->dadosDaLegenda();
                foreach ($legenda as $value) {
                    echo <<<HTML
                        <li>
                            <table>
                                <tr>
                                    <td style="width:47px;">
                                        <span style="background:{$value['cor']};" class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>{$value['total']}</b>&nbsp;&nbsp;
                                    </td>
                                    <td>{$value['descricao']}</td>
                                </tr>
                            </table>
                        </li>
HTML;
                }
                ?>
            </ul>
        </div>
    <?php
    }
}