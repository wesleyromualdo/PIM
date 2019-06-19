<?php

/**
 * classe Mapas
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 *
 * Objeto que retorna informações (dados em formato geoJSON) direcionadas a popular os mapas
 */
class Mapas extends Modelo{

    /**
     * O mapa deve exibir informações de adequação.
     */
    const TIPO_ADEQUACAO = 1;
    /**
     * O mapa deve exibir informações da lei pne.
     */
    const TIPO_LEIPNE = 2;
    /**
     * O mapa deve exibir informações de adesão.
     */
    const TIPO_ADESAO = 3;

    //definição constantes sase_mapa_planodecarreira
    const PLANO_CARREIRA = 238;
    const PCR            = 242;
    const ADESAO         = 244;
    const ATO            = 246;
    const ADEQUACAO      = 240;

    public $Solicitacao;

    protected $Joins = array();

	// atributos ligados a requisicao
    public $origemRequisicao;
    public $params;
    public $chamadoMapas;

	// Partes dos Mapas
		protected $Poligonos;
		protected $MetaDados;

	/**
	 *	Construct
	 */
	public function __construct(){
            $this->Poligonos = new Poligonos();
            $this->MetaDados = new MetaDados();
	}

	/**
	 * Retorna o resultado da requisicao
	 *
	 * @todo testar
	 */
	public function requisicaoPost(){
		if( $this->params['estado'] )
			$this->params['estado'] = $this->garanteQueSejaArray( $this->params['estado'] );
		if( $this->params['municipio'] )
			$this->params['municipio'] = $this->garanteQueSejaArray( $this->params['municipio'] );

		$this->construtorPoligonos(); // montaEstado, montaEstadoLegenda ...

		$this->construtorMetaDados(); // aplica cores aos dados em json.

		// escreve resultado
		ob_clean();
		echo $this->Poligonos->poligono;
		die();

	}

	/**
	 * Construtor de Poligonos
	 *
	 * @internal aqui encapsulam as regras da construção de polígonos
	 *			 - praticamente baseadas em $this->origemRequisicao
	 * @internal Factory Method
	 * @return void
	 * @internal preenche os respectivos objetos que compoem a solicitação
	 */
	protected function construtorPoligonos(){
            switch ( $this->origemRequisicao ) {
                case 'monitoramento':
                case 'agenda-trabalho':
                case 'assessoramento':
                case 'assessoramento-lei-municipio':
                case 'assessoramento-adesao-municipio':
                case 'assessoramento-externo':
                case 'assessoramento-externo-adesao':
                case 'planocarreira-municipio':
                case 'acompanhamento-pne-municipio':
                case 'plano-carreira-externo':
                case 'monitoramentopne-municipio-externo':
                case 'espacodeparticipacao-municipio':
                case 'ficha-tecnica-educacao-municipio-externo':
                case 'agenda-trabalho-municipio-externo':
                    $this->Poligonos->estuf = $this->params['estado'];
                    $this->Poligonos->muncod = $this->params['municipio'];

                    $teste = $this->Poligonos->getMunicipios();
                    break;

                case 'monitoramento-estado':
                case 'agenda-trabalho-estado':
                case 'assessoramento-estado':
                case 'assessoramento-lei-estado':
                case 'assessoramento-adesao-estado':
                case 'planocarreira-estado':
                case 'acompanhamento-pne-estado':
                case 'sase-mapas-estado-externo':
                case 'espacodeparticipacao-estado':
                    $this->Poligonos->estuf = $this->params['estado'];
                    $this->Poligonos->muncod = $this->params['municipio'];
                    $this->Poligonos->getEstado();
                    break;

                case 'questoespontuais':
                case 'questoespontuais-externo':
                    $this->Poligonos->estuf = $this->params['estado'];
                    //$this->Poligonos->muncod = $this->params['municipio'];
                    $this->Poligonos->getMunicipios();
                    break;

                case 'organizacoesterritoriais':
                    $this->Poligonos->orgid = $this->params['orgid'];

                    if($this->params['municipio']){
                        $this->Poligonos->muncod = array($this->params['municipio']);
                    }if($this->params['estado']){
                        $this->Poligonos->estuf = $this->params['estado'];
                    }if($this->params['mesid']){
                        $this->Poligonos->mesid = $this->params['mesid'];
                    }
                    $this->Poligonos->getMunicipios();
                    break;

                case 'organizacoesterritoriais-externo':
                    $this->Poligonos->estuf = $this->params['estado'];
                    $this->Poligonos->orgid = $this->params['orgid'];

                    $this->Poligonos->getMunicipios();
                    break;

                case 'pais-externo':
                case 'planocarreira-estado-externo':
                case 'plano-carreira-estado-externo':
                case 'monitoramentopne-estado-externo':
                case 'monitoramento-estado-externo':
                    $this->Poligonos->getPais();
                    break;
                case 'agenda-trabalho-estado-externo':
                    $this->Poligonos->getPais();
                    break;

                case 'municipio-externo':
                    $this->Poligonos->getMunicipios();
                    break;
                case 'plano-carreira-municipio-externo':
                case 'monitoramento-externo':
                case 'agenda-trabalho-externo':
                    $this->Poligonos->getMunicipios();
                    break;
            }

	}

	/**
	 * Construtor de MetaDados
	 *
	 * @interplano-carreira-municipio-externonal aqui encapsulam as regras da construção de metadados
	 * @internal Factory Method
	 * @internal método $this->MetaDados->aplicaCoresAosMunicipios adicionado no switch pq para
	 *			 cada origem pode demandar uma requisicao diferente.
	 * @return void
	 */
	protected function construtorMetaDados()
    {


        switch ($this->origemRequisicao) {
            case 'monitoramento':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado( 'DadoFichaMonitoramento' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;
            case 'monitoramento-estado':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado('DadosFichaMonitoramentoEstado');
                $this->MetaDados->aplicaCoresAosEstados( $this->Poligonos );
                break;
            case 'agenda-trabalho':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado( 'DadoAgendaTrabalhoComissaoCoordenadora' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;

            case 'agenda-trabalho-estado':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado( 'DadoAgendaTrabalhoComissaoCoordenadoraEstado' );
                $this->MetaDados->aplicaCoresAosEstados( $this->Poligonos );
                break;

            case 'assessoramento-estado':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado( 'DadoSituacaoAssessoramentoEstado' );
                $this->MetaDados->aplicaCoresAosEstados( $this->Poligonos );
                break;

            case 'assessoramento-lei-estado':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado( 'DadoSituacaoAssessoramentoEstadoLei' );
                $this->MetaDados->aplicaCoresAosEstados( $this->Poligonos );
                break;

            case 'assessoramento-adesao-estado':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado( 'DadoSituacaoAssessoramentoEstadoAdesao' );
                $this->MetaDados->aplicaCoresAosEstados( $this->Poligonos );
                break;

            case 'assessoramento-lei-municipio':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado( 'DadoSituacaoAssessoramentoMunicipioLeiPNE' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;

            case 'assessoramento-adesao-municipio':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado( 'DadoSituacaoAssessoramentoMunicipioAdesao' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;

            case 'assessoramento':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado( 'DadoSituacaoAssessoramento' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;

            case 'assessoramento-externo':
            case 'assessoramento-externo-adesao':
                $this->MetaDados->estuf = $this->params['estado'];
                switch ($this->params['tipoinfo']) {
                    case self::TIPO_LEIPNE:
                        $classeAssessoramentoExterno = 'DadoMacroCategoriaLeipne';
                        break;
                    case self::TIPO_ADESAO:
                        $classeAssessoramentoExterno = 'DadoMacroCategoriaAdesao';
                        break;
                    case self::TIPO_ADEQUACAO:
                    default:
                        $classeAssessoramentoExterno = 'DadoMacroCategoria';
                        break;
                }

                $this->MetaDados->montaDado($classeAssessoramentoExterno);
                $this->MetaDados->aplicaCoresAosMunicipios($this->Poligonos);
                break;

            case 'plano-carreira-externo':
//                $this->MetaDados->estuf = $this->params['estado'];
//                
//                $this->MetaDados->montaDado( 'DadoMacroPlanoCarreiraExterno' );
//                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );                

                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->tpdid = $_SESSION['tipo'];
                $this->MetaDados->montaDado( 'DadoPlanoCarreiraMunicipio' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;
            case 'planocarreira-municipio':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->tpdid = $this->params['tpdid'];

                $this->MetaDados->montaDado( 'DadoPlanoCarreiraMunicipio' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;

            case 'planocarreira-estado':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->tpdid = $this->params['tpdid'];
                $this->MetaDados->montaDado( 'DadoPlanoCarreiraEstado' );
                $this->MetaDados->aplicaCoresAosEstados( $this->Poligonos );
                break;

            case 'monitoramentopne-municipio-externo':
                $this->MetaDados->estuf  = $this->params['estado'];
                $this->MetaDados->subid  = $this->params['subid'];
                $this->MetaDados->montaDado( 'DadoMonitoramentoPneMunicipioExterno' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                $this->MetaDados->aplicaDadosComplementares( array('pnevalor', 'pneano'), $this->Poligonos );
                break;

            case 'acompanhamento-pne-municipio':
                $this->MetaDados->estuf  = $this->params['estado'];
                $this->MetaDados->muncod  = $this->params['municipio'];
                $this->MetaDados->subid  = $this->params['subid'];
                $this->MetaDados->pneciclo = $this->params['pneciclo'];
                $this->MetaDados->montaDado( 'DadoAcompanhoPneMunicipio' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;

            case 'espacodeparticipacao-municipio':
                $this->MetaDados->estuf  = $this->params['estado'];
                $this->MetaDados->tpmapa  = $this->params['tpmapa'];
                $this->MetaDados->muncod = $this->params['muncod'];
                $this->MetaDados->montaDado( 'DadoEspacoParticipacaoMunicipio' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;

            case 'espacodeparticipacao-estado':
                $this->MetaDados->estuf  = $this->params['estado'];
                $this->MetaDados->tpmapa  = $this->params['tpmapa'];
                $this->MetaDados->montaDado( 'DadoEspacoParticipacaoEstado' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;

            case 'monitoramentopne-estado-externo':
                $this->MetaDados->subid  = $this->params['subid'];
                $this->MetaDados->montaDado( 'DadoMonitoramentoPneEstadoExterno' );
                $this->MetaDados->aplicaCoresAosEstados( $this->Poligonos );
                break;

            case 'acompanhamento-pne-estado':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->subid  = $this->params['subid'];
                $this->MetaDados->pneciclo = $this->params['pneciclo'];
                $this->MetaDados->montaDado( 'DadoAcompanhoPneEstado' );
                $this->MetaDados->aplicaCoresAosEstados( $this->Poligonos );
                break;

            case 'questoespontuais':
            case 'questoespontuais-externo':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado( 'DadoSituacaoQuestoesPontuais' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;

            case 'organizacoesterritoriais':
                $this->MetaDados->orgid = $this->params['orgid'];
                if($this->params['municipio']){
                    $this->MetaDados->muncod = $this->params['municipio'];
                }
                if($this->params['estado']){
                    $this->MetaDados->estuf = $this->params['estado'];
                }
                $this->MetaDados->montaDado( 'DadoMesoregiao' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;
            case 'organizacoesterritoriais-externo':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->orgid = $this->params['orgid'];
                $this->MetaDados->montaDado( 'DadoMesoregiao' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;

            case 'pais-externo':
                switch ($this->params['tipoinfo']) {
                    case self::TIPO_LEIPNE:
                        $classePaisExterno = 'DadoEstadosPaisLeipne';
                        break;
                    case self::TIPO_ADESAO:
                        $classePaisExterno = 'DadoEstadosPaisAdesao';
                        break;
                    case self::TIPO_ADEQUACAO:
                    default:
                        $classePaisExterno = 'DadoEstadosPais';
                        break;
//                        throw new Exception("Tipo de informação desconhecido ({$this->params['tipoinfo']}).");
                }

                $this->MetaDados->montaDado($classePaisExterno);
                $this->MetaDados->aplicaCoresAosEstados($this->Poligonos);
                break;
            //preenche mapa sase_mapa_planodecarreira
            case 'planocarreira-estado-externo':
                $this->MetaDados->tpdid = $this->params['tpdid'];
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado('DadoPlanocarreiraEstadoExterno');
                $this->MetaDados->aplicaCoresAosEstados( $this->Poligonos );
                break;

            case 'municipio-externo':
                switch ($this->params['tipoinfo']) {
                    case self::TIPO_LEIPNE:
                        $classeMuniciopExterno = 'DadoSituacaoAssessoramentoLeipne';
                        break;
                    case self::TIPO_ADESAO:
                        $classeMuniciopExterno = 'DadoSituacaoAssessoramentoAdesao';
                        break;
                    case self::TIPO_ADEQUACAO:
                    default:
                        $classeMuniciopExterno = 'DadoSituacaoAssessoramento';
                        break;
//                        throw new Exception("Tipo de informação desconhecido ({$this->params['tipoinfo']}).");
                }

                $this->MetaDados->montaDado($classeMuniciopExterno);
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;
            case 'plano-carreira-municipio-externo':
                switch ($this->params['tipoinfo']) {
                    case self::PCR:
                        $classeMuniciopExterno = 'DadosPlanoCarreiraPCR';
                        break;
                    case self::ADESAO:
                        $classeMuniciopExterno = 'DadosPlanoCarreiraAdesao';
                        break;
                    case self::ATO:
                        $classeMuniciopExterno = 'DadosPlanoCarreiraAto';
                        break;
                    case self::PLANO_CARREIRA:
                        $classeMuniciopExterno = 'DadosPlanoCarreira';
                        break;
                    case self::ADEQUACAO:
                    default:
                        $classeMuniciopExterno = 'DadosPlanoCarreiraAdequacao';
                        break;
//                        throw new Exception("Tipo de informação desconhecido ({$this->params['tipoinfo']}).");
                }

                $this->MetaDados->montaDado($classeMuniciopExterno);
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;
            case 'plano-carreira-estado-externo':


                switch ($this->params['tipoinfo']) {
                    case self::PCR:
                        $classePCarreiraEstadoExterno = 'DadosEstadosPlanoCarreiraPCR';
                        break;
                    case self::ADESAO:
                        $classePCarreiraEstadoExterno = 'DadosEstadosPlanoCarreiraAdesao';
                        break;
                        case self::ATO:
                        $classePCarreiraEstadoExterno = 'DadosEstadosPlanoCarreiraAto';
                        break;
                    case self::PLANO_CARREIRA:
                        $classePCarreiraEstadoExterno = 'DadosEstadosPlanoCarreira';
                        break;
                    case self::ADEQUACAO:
                    default:
                        $classePCarreiraEstadoExterno = 'DadosEstadosPlanoCarreiraAdequacao';
                        break;
                }

                $this->MetaDados->montaDado($classePCarreiraEstadoExterno);
                $this->MetaDados->aplicaCoresAosEstados($this->Poligonos);
                break;
            case 'monitoramento-externo':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado('DadoFichaMonitoramentoExterno');
                $this->MetaDados->aplicaCoresAosMunicipios($this->Poligonos);
                break;
            case 'monitoramento-estado-externo':
                $this->MetaDados->montaDado('DadoFichaMonitoramentoEstadoExterno');
                $this->MetaDados->aplicaCoresAosEstados($this->Poligonos);
                break;
            case 'agenda-trabalho-externo':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado('DadoAgendaComissaoCoordenadoraExterno');
                $this->MetaDados->aplicaCoresAosMunicipios($this->Poligonos);
                break;
            case 'agenda-trabalho-estado-externo':
                $this->MetaDados->montaDado('DadoAgendaEstadoComissaoCoordenadoraExterno');
                $this->MetaDados->aplicaCoresAosEstados($this->Poligonos);
                break;

            case 'ficha-tecnica-educacao-municipio-externo':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado( 'DadoFichaMonitoramentoExterno' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;

            case 'agenda-trabalho-municipio-externo':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado( 'DadoAgendaComissaoCoordenadoraExterno' );
                $this->MetaDados->aplicaCoresAosMunicipios( $this->Poligonos );
                break;

        }
	}

	/**
	 * Garante que variável seja array
	 *
	 * @param $var
	 * @return $array
	 */
	protected function garanteQueSejaArray( $var ){
		if(
			$var
			&& !is_array($var)
		)
			$var = array($var);
		return $var;
	}

	/**
	 *
	 */
	public function requisicaoPostLegenda(){
            global $db;
            if( count($this->params['estados']) > 0 ){
                ob_clean();
                switch(	$this->origemRequisicao ){
                    case 'assessoramento-legenda-estado':

                        include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
                        $SituacaoAssessoramento = new SituacaoAssessoramento();
                        $SituacaoAssessoramento->montaLegendaEstado( (( !is_array($this->params['estados']) )?array():$this->params['estados']) );

                    break;
                    case 'assessoramento-legenda-lei-estado':

                        include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
                        $SituacaoAssessoramento = new SituacaoAssessoramento();
                        $SituacaoAssessoramento->montaLegendaLeiAdesaoEstado( (( !is_array($this->params['estados']) )?array():$this->params['estados']), 'lei');

                    break;
                    case 'assessoramento-legenda-adesao-estado':

                        include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
                        $SituacaoAssessoramento = new SituacaoAssessoramento();
                        $SituacaoAssessoramento->montaLegendaLeiAdesaoEstado( (( !is_array($this->params['estados']) )?array():$this->params['estados']), 'adesao');

                    break;
                    case 'planocarreira-municipio-legenda':

                        include APPRAIZ . 'sase/classes/Planodecarreira.class.inc';
                        $planodecarreira = new Planodecarreira();
                        $planodecarreira->getLegenda((( !is_array($this->params['estados']) )?array():$this->params['estados']), $this->params['tpdid']);

                    break;
                    case 'planocarreira-estado-legenda':

                        include APPRAIZ . 'sase/classes/PlanodecarreiraEstado.class.inc';
                        $planodecarreira = new PlanodecarreiraEstado();
                        $planodecarreira->getLegenda((( !is_array($this->params['estados']) )?array():$this->params['estados']), $this->params['tpdid']);

                    break;

                    case 'assessoramento-legenda':

                        include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
                        $SituacaoAssessoramento = new SituacaoAssessoramento();
                        $SituacaoAssessoramento->montaLegenda( (( !is_array($this->params['estados']) )?array():$this->params['estados']) );

                    break;
                    case 'monitoramento-legenda':

                        include_once APPRAIZ . 'sase/classes/Fichamonitoramento.class.inc';
                        $monitoramento = new Fichamonitoramento();
                        $monitoramento->montaLegenda( (( !is_array($this->params['estados']) )?array():$this->params['estados']) );

                    break;
                    case 'monitoramento-legenda-estado':

                        include_once APPRAIZ . 'sase/classes/FichamonitoramentoEstado.class.inc';
                        $monitoramentoEstado = new Fichamonitoramentoestado();
                        $monitoramentoEstado->montaLegendaEstado( (( !is_array($this->params['estados']) )?array():$this->params['estados']) );

                    break;
                    case 'agenda-trabalho-legenda':

                        include_once APPRAIZ . 'sase/classes/Model/Etapaagendacomcoord.class.inc';
                        $agenda = new Sase_Model_Etapaagendacomcoord();
                        $agenda->montaLegenda( (( !is_array($this->params['estados']) )?array():$this->params['estados']) );

                    break;
                    case 'agenda-trabalho-legenda-estado':

                        include_once APPRAIZ . 'sase/classes/Model/Agendacomissaocoordestado.class.inc';
                        $agendaEstado = new Agendacomissaocoordestado();
                        $agendaEstado->montaLegendaEstado( (( !is_array($this->params['estados']) )?array():$this->params['estados']) );

                    break;
                    case 'assessoramento-lei-municipio':

                        include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
                        $SituacaoAssessoramento = new SituacaoAssessoramento();
                        $SituacaoAssessoramento->montaLegendaLeiMunicipio( (( !is_array($this->params['estados']) )?array():$this->params['estados']) );

                    break;
                    case 'assessoramento-adesao-municipio':

                        include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
                        $SituacaoAssessoramento = new SituacaoAssessoramento();
                        $SituacaoAssessoramento->montaLegendaAdesaoMunicipio( (( !is_array($this->params['estados']) )?array():$this->params['estados']) );

                    break;
                    case 'espacodeparticipacao-municipio-legenda':
//                        include_once APPRAIZ . 'sase/classes/Mapa/MetaDados/DadoEspacoParticipacaoMunicipio.class.inc';
//                        $dados = new DadoEspacoParticipacaoMunicipio();
//                        $dados->dadosDaLegenda();
                        getLegendaEspacoParticipacaoMunicipio($this->params['tpmapa'], $this->params['estados']);
                    break;
                    case 'assessoramento-legenda-externo':
                        include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
                        $SituacaoAssessoramento = new SituacaoAssessoramento();
                        $estados = ((!is_array($this->params['estados']))?[]:$this->params['estados']);

                        $SituacaoAssessoramento->montaLegendaMacroCategoria($estados, $this->params['tipoinfo']);
                    break;
                    case 'plano-carreira-legenda-externo':
                        include_once APPRAIZ . 'sase/classes/Sitplancarprofessor.class.inc';
                        $sit  = new Sitplancarprofessor();
                        $sit->montaLegendaMapaExterno((( !is_array($this->params['estados']) )?array():$this->params['estados']));
                    break;
                    case 'pais-legenda-externo':
                        include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
                        $SituacaoAssessoramento = new SituacaoAssessoramento();
                        $SituacaoAssessoramento->montaLegendaMacroCategoriaPais($this->params['tipoinfo']);
                    break;
                    case 'monitoramentopne-municipio-legenda-externo':
                    case 'monitoramentopne-estado-legenda-externo':
                        $estado = isset($this->params['estuf']) ? $this->params['estuf'] : '';
                        getLegendaMonitoramentoPneMapaExterno($this->params['subid'], $estado);
                        break;
                    case 'planocarreira-estado-legenda-externo':

                        include APPRAIZ . 'sase/classes/PlanodecarreiraEstado.class.inc';
                        $planodecarreira = new PlanodecarreiraEstado();
                        $planodecarreira->getLegenda(array(), $this->params['tpdid']);

                        break;
                    case 'planocarreira-municipio-legenda-externo':

                        include APPRAIZ . 'sase/classes/Planodecarreira.class.inc';
                        $planodecarreira = new Planodecarreira();
                        $planodecarreira->getLegenda((( !is_array($this->params['estados']) )?array():$this->params['estados']), $this->params['tpdid']);

                        break;
                    case 'municipio-legenda-externo':
                        include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
                        $SituacaoAssessoramento = new SituacaoAssessoramento();
                        $SituacaoAssessoramento->montaLegendaMunicipioExterno($this->params['tipoinfo']);

                    break;
                    case 'questoespontuais-legenda':
                    case 'questoespontuais-legenda-externo':

                        include_once APPRAIZ . 'sase/classes/QuestoesPontuaisPar.class.inc';
                        $QuestoesPontuaisPar = new QuestoesPontuaisPar();
                        $QuestoesPontuaisPar->montaLegenda( (( !is_array($this->params['estados']) )?array():$this->params['estados']) );

                    break;
                    case 'organizacoesterritoriais-legenda':
                        include_once APPRAIZ . 'sase/classes/OrganizacoesTerritoriais.class.inc';
                        $organizacoesTerritoriais= new OrganizacoesTerritoriais();

                        $ufs = (( !is_array($this->params['estados']) )?array():$this->params['estados']);
                        $orgid = (( !$this->params['orgid'] ) ? null :$this->params['orgid']);
                        $muncod = (( !$this->params['municipio'] ) ? null :$this->params['municipio']);

                        $organizacoesTerritoriais->montaLegenda($ufs, $orgid, $muncod);
                        break;

                    case 'organizacoesterritoriais-legenda-externo':

                        include_once APPRAIZ . 'sase/classes/Territorio.class.inc';
                        $Territorio = new Territorio();
                        $Territorio->montaLegenda( (( !is_array($this->params['estados']) )?array():$this->params['estados']), (( !$this->params['orgid'] )?'':$this->params['orgid']) );

                    break;
                    case 'plano-carreira-municipio-legenda-externo':
                        include_once APPRAIZ . 'sase/classes/Planodecarreira.class.inc';
                        $planocarreiramun = new Planodecarreira();
                        $planocarreiramun->montaLegendaMunicipioExterno($this->params['tipoinfo']);

                    break;
                    case 'plano-carreira-estado-legenda-externo':
                        include_once APPRAIZ . 'sase/classes/PlanodecarreiraEstado.class.inc';
                        $planocarreiraest = new PlanodecarreiraEstado();
                        $planocarreiraest->montaLegendaPlanoCarreiraEstados($this->params['tipoinfo']);

                    break;
                    case 'monitoramento-legenda-externo':
                        include_once APPRAIZ . 'sase/classes/Fichamonitoramento.class.inc';
                        $fichamun = new Fichamonitoramento();
                        $estados = ((!is_array($this->params['estados']))?[]:$this->params['estados']);
                        $fichamun->montaLegendaMunicipioExterno($estados);

                    break;
                    case 'monitoramento-legenda-estado-externo':
                        include_once APPRAIZ . 'sase/classes/FichamonitoramentoEstado.class.inc';
                        $fichaest  = new FichamonitoramentoEstado();
                        $fichaest->montaLegendaEstadoExterno((( !is_array($this->params['estados']) )?array():$this->params['estados']));
                    break;
                    case 'agenda-trabalho-legenda-externo':
                        include_once APPRAIZ . 'sase/classes/Model/Etapaagendacomcoord.class.inc';
                        $agendmun = new Sase_Model_Etapaagendacomcoord();
                        $estados = ((!is_array($this->params['estados']))?[]:$this->params['estados']);
                        $agendmun->montaLegendaMunicipioExterno($estados);

                    break;
                    case 'agenda-trabalho-legenda-estado-externo':
                        include_once APPRAIZ . 'sase/classes/Model/Agendacomissaocoordestado.class.inc';
                        $agendEst = new Agendacomissaocoordestado();
                        $agendEst->montaLegendaEstadoExterno((( !is_array($this->params['estados']) )?array():$this->params['estados']));
                    break;

                    case 'ficha-tecnica-educacao-municipio-legenda-externo':
                        include_once  APPRAIZ .'sase/classes/Model/Fichamonitoramento.php';
                        $leg = new Sase_Model_Fichamonitoramento();

                        $leg->montaLegendaFichaMonitoramentoExterno((( !is_array($this->params['estados']) )?array():$this->params['estados']));
                        break;

                    default:
                    break;
                }
                die();
		}

	}

	/**
	 * Componentes em _componentes.php
	 *
	 * @param $componentes - nome da função
	 * @param $params - parametros a serem passados para funcao
	 */
	public function componente( $componente, $params ){
		$funcao = $componente.'("'.implode('","', $params).'");';
		eval( $funcao );

	}

}

// ############################################################################ CAPTURA DE REQUISICAO POST
$chamadoMapas = new Mapas();

if( $_POST['chamadoMapas'] ){
	// if( $_POST['origemRequisicao']=='sase-mapas-estado-externo' ){ print_r($_POST['params']);exit; }

	$chamadoMapas->origemRequisicao = $_POST['origemRequisicao'];
	$chamadoMapas->params = $_POST['params'];
	$chamadoMapas->chamadoMapas = $_POST['chamadoMapas'];
	if( $_POST['chamadoMapas'] == 'legenda' ){
		$chamadoMapas->requisicaoPostLegenda();

	}else if( $_POST['chamadoMapas'] == 'componente-buscamunicipios' ){
		$chamadoMapas->componente('BuscaMunicipio', array('estuf' => $chamadoMapas->params) );

	}else
		$chamadoMapas->requisicaoPost();

}
// ########################################################################################################

