<?php
/**
 * Classe de mapeamento da entidade emenda.carga_emenda_siop.
 *
 * @version $Id$
 * @since 2017.08.04
 */

/**
 * Emenda_Model_Carga_emenda_siop: sem descricao
 *
 * @package Emenda\Model
 * @uses Simec\Db\Modelo
 * @author Victor Martins Machado <adesia@ufba.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Emenda_Model_Carga_emenda_siop($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Emenda_Model_Carga_emenda_siop($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $prioridade Prioridade da emenda
 * @property string $ptres PTRES
 * @property numeric $valorimpedido Valor impedido do beneficiário
 * @property numeric $limiteempenhado Limite Empenhado
 * @property numeric $dotacaoatualizada Dotação Atualizada
 * @property string $mapcod Modalidade
 * @property smallint $gnd Código do GND
 * @property string $planoorcamentario Plano Orçamentário
 * @property string $autcod Código do parlamentar
 * @property string $ano_emenda 
 * @property numeric $valor_disponivel 
 * @property numeric $valor_rcl_apurada 
 * @property numeric $valor_aprovado 
 * @property string $nome_beneficiario 
 * @property string $cnpj 
 * @property numeric $valor_emenda 
 * @property string $uf 
 * @property string $partido 
 * @property string $nome_parlamentar 
 * @property string $emenda 
 * @property string $idoc 
 * @property string $iduso 
 * @property string $fonte 
 * @property string $rp_atual 
 * @property string $natureza_despesa 
 * @property string $localizador 
 * @property string $acao 
 * @property bigint $programa 
 * @property string $subfuncao 
 * @property string $funcao 
 * @property string $uo 
 * @property string $esfera 
 * @property string $momento 
 */
class Emenda_Model_Carga_emenda_siop extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'emenda.carga_emenda_siop';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'prioridade' => null,
        'ptres' => null,
        'valorimpedido' => null,
        'limiteempenhado' => null,
        'dotacaoatualizada' => null,
        'mapcod' => null,
        'gnd' => null,
        'planoorcamentario' => null,
        'autcod' => null,
        'ano_emenda' => null,
        'valor_disponivel' => null,
        'valor_rcl_apurada' => null,
        'valor_aprovado' => null,
        'nome_beneficiario' => null,
        'cnpj' => null,
        'valor_emenda' => null,
        'uf' => null,
        'partido' => null,
        'nome_parlamentar' => null,
        'emenda' => null,
        'idoc' => null,
        'iduso' => null,
        'fonte' => null,
        'rp_atual' => null,
        'natureza_despesa' => null,
        'localizador' => null,
        'acao' => null,
        'programa' => null,
        'subfuncao' => null,
        'funcao' => null,
        'uo' => null,
        'esfera' => null,
        'momento' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'prioridade' => array('allowEmpty' => true,'Digits'),
            'ptres' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 50))),
            'valorimpedido' => array('allowEmpty' => true),
            'limiteempenhado' => array('allowEmpty' => true),
            'dotacaoatualizada' => array('allowEmpty' => true),
            'mapcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'gnd' => array('allowEmpty' => true),
            'planoorcamentario' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4))),
            'autcod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'ano_emenda' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 4))),
            'valor_disponivel' => array('allowEmpty' => true),
            'valor_rcl_apurada' => array('allowEmpty' => true),
            'valor_aprovado' => array('allowEmpty' => true),
            'nome_beneficiario' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'cnpj' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 15))),
            'valor_emenda' => array('allowEmpty' => true),
            'uf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'partido' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'nome_parlamentar' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'emenda' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'idoc' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'iduso' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'fonte' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'rp_atual' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'natureza_despesa' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'localizador' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'acao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'programa' => array('allowEmpty' => true),
            'subfuncao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'funcao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5))),
            'uo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'esfera' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
            'momento' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    /**
     * Função _inserir
     * Método usado para inserção de um registro do banco
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @access private
     * @author Orion Teles de Mesquita
     * @since 12/02/2009
     */
    public function inserir($arCamposNulo = array())
    {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
        if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );
        $arCampos  = array();
        $arValores = array();
        $arSimbolos = array();

        $troca = array("'", "\\");
        foreach( $this->arAtributos as $campo => $valor ){
            if( $campo == $this->arChavePrimaria[0] && !$this->tabelaAssociativa ) continue;
            if( $valor !== null ){
                if( !$valor && in_array($campo, $arCamposNulo) ){ continue; }
                $arCampos[]  = $campo;
                $valor = str_replace($troca, "", $valor);
                $arValores[] = trim( pg_escape_string( $valor ) );
            }
        }

        if( count( $arValores ) ){
            $sql = " insert into $this->stNomeTabela ( ". implode( ', ', $arCampos   ) ." )
											  values ( '". implode( "', '", $arValores ) ."' )";
        }
        return $this->executar( $sql ) ? true : false;
    } // Fim _inserir()

    /**
     * Função _inserir mantendo aspas
     * Método usado para inserção de um registro do banco
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @access private
     * @author
     * @since 09/10/2015
     */
    public function inserirManterAspas($arCamposNulo = array())
    {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
        if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );

        $arCampos  = array();
        $arValores = array();
        $arSimbolos = array();

        foreach( $this->arAtributos as $campo => $valor ){
            if( $campo == $this->arChavePrimaria[0] && !$this->tabelaAssociativa ) continue;
            if( $valor !== null ){
                if( !$valor && in_array($campo, $arCamposNulo) ){ continue; }
                $arCampos[]  = $campo;
                $arValores[] = trim( pg_escape_string( $valor ) );
            }
        }

        if( count( $arValores ) ){
            $sql = " insert into $this->stNomeTabela ( ". implode( ', ', $arCampos   ) ." )
											  values ( '". implode( "', '", $arValores ) ."' )";
            return $this->executar( $sql ) ? true : false;
        }
    } // Fim _inserirManterAspas()

    /**
     * Função salvar
     * Método usado para inserção ou alteração de um registro do banco
     * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
     * @access public
     * @author Orion Teles de Mesquita
     * @since 12/02/2009
     */
    public function salvar($boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array(), $manterAspas = false){
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();

        if( $boAntesSalvar ){
            if( !$this->antesSalvar() ){ return false; }
        }

        if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor declarar atributo 'protected \$arChavePrimaria = [];' na classe filha!" );

        $stChavePrimaria = $this->arChavePrimaria[0];
//        $this->validar($this->arAtributos);

//        if( $this->$stChavePrimaria && !$this->tabelaAssociativa ){
//            $this->alterar($arCamposNulo);
//            $resultado = $this->$stChavePrimaria;
//        }else{
            if($manterAspas === false){
                $resultado = $this->inserir($arCamposNulo);
            }else{
                $resultado = $this->inserirManterAspas($arCamposNulo);
            }
//        }
        if( $resultado ){
            if( $boDepoisSalvar ){
                $this->depoisSalvar();
            }
        }
        return $resultado;
    } // Fim salvar()
    
    public function limpaTabela(){        
        $sql = "TRUNCATE {$this->stNomeTabela}";
        return $this->executar( $sql ) ? true : false;
    }
}
