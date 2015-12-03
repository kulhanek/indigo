﻿using System;
using System.Collections.Generic;
using System.Text;
namespace com.ggasoftware.indigo
{
   public unsafe interface IndigoLib
   {
      sbyte* indigoVersion ();
      long indigoAllocSessionId ();
      void indigoSetSessionId (long id);
      void indigoReleaseSessionId (long id);
      sbyte* indigoGetLastError ();
      void indigoSetErrorHandler (Indigo.ErrorHandler handler, Indigo context);
      void indigoSetErrorMessage (String message);
      int indigoFree (int id);
      int indigoClone (int id);
      int indigoCountReferences ();

      int indigoSetOption (string name, string value);
      int indigoSetOptionInt (string name, int value);
      int indigoSetOptionBool (string name, int value);
      int indigoSetOptionFloat (string name, float value);
      int indigoSetOptionColor (string name, float r, float g, float b);
      int indigoSetOptionXY (string name, int x, int y);

      int indigoReadFile (string filename);
      int indigoReadString (string str);
      int indigoLoadString (string str);
      int indigoReadBuffer (string buffer, int size);
      int indigoLoadBuffer (string buffer, int size);
      int indigoWriteFile (string filename);
      int indigoWriteBuffer ();
      int indigoClose (int item);

      int indigoCreateMolecule ();
      int indigoCreateQueryMolecule ();

      int indigoLoadMoleculeFromString (string str);
      int indigoLoadMoleculeFromFile (string path);
      int indigoLoadMoleculeFromBuffer (byte[] buf, int bufsize);
      int indigoLoadQueryMoleculeFromString (string str);
      int indigoLoadQueryMoleculeFromFile (string path);
      int indigoLoadQueryMoleculeFromBuffer (byte[] buf, int bufsize);
      int indigoLoadSmartsFromString (string str);
      int indigoLoadSmartsFromFile (string filename);
      int indigoLoadSmartsFromBuffer (byte[] buffer, int size);
      int indigoSaveMolfile (int molecule, int output);
      int indigoSaveMolfileToFile (int molecule, string filename);
      sbyte* indigoMolfile (int molecule);
      int indigoSaveCml (int molecule, int output);
      int indigoSaveCmlToFile (int molecule, string filename);
      sbyte* indigoCml (int molecule);
      int indigoSaveMDLCT (int item, int output);

      int indigoCreateSaver (int output, string format);
      int indigoCreateFileSaver (string filename, string format);
      int indigoAppend (int saver, int obj);

      int indigoLoadReactionFromString (string str);
      int indigoLoadReactionFromFile (string path);
      int indigoLoadReactionFromBuffer (byte[] buf, int bufsize);
      int indigoLoadQueryReactionFromString (string str);
      int indigoLoadQueryReactionFromFile (string path);
      int indigoLoadQueryReactionFromBuffer (byte[] buf, int bufsize);
      int indigoLoadReactionSmartsFromString (string str);
      int indigoLoadReactionSmartsFromFile (string path);
      int indigoLoadReactionSmartsFromBuffer (byte[] buf, int bufsize);
      int indigoCreateReaction ();
      int indigoCreateQueryReaction ();
      int indigoAddReactant (int reaction, int molecule);
      int indigoAddProduct  (int reaction, int molecule);
      int indigoAddCatalyst (int reaction, int molecule);
      int indigoCountReactants (int reaction);
      int indigoCountProducts (int reaction);
      int indigoCountCatalysts (int reaction);
      int indigoCountMolecules (int reaction);
      int indigoIterateReactants (int reaction);
      int indigoIterateProducts  (int reaction);
      int indigoIterateCatalysts (int reaction);
      int indigoIterateMolecules (int reader);
      int indigoSaveRxnfile (int reaction, int output);
      int indigoSaveRxnfileToFile (int reaction, string filename);
      sbyte* indigoRxnfile (int reaction);
      int indigoAutomap (int reaction, string filename);
      int indigoGetAtomMappingNumber (int reaction, int reaction_atom);
      int indigoSetAtomMappingNumber (int reaction, int reaction_atom, int number);
      int indigoClearAAM (int reaction);
      int indigoCorrectReactingCenters (int reaction);

      int indigoGetReactingCenter (int reaction, int reaction_bond, int *rc);
      int indigoSetReactingCenter (int reaction, int reaction_bond, int rc);

      int indigoOptimize (int query, string options);

      int indigoIterateAtoms (int molecule);
      int indigoIteratePseudoatoms (int molecule);
      int indigoIterateRSites (int molecule);
      int indigoIterateStereocenters (int molecule);
      int indigoIterateAlleneCenters (int molecule);
      int indigoIterateRGroups (int molecule);
      int indigoIterateRGroupFragments (int rgroup);
      int indigoCountAttachmentPoints (int rgroup);
      int indigoIsPseudoatom (int atom);
      int indigoIsRSite (int atom);
      int indigoStereocenterType (int atom);
      int indigoSingleAllowedRGroup (int rsite);

      int indigoChangeStereocenterType (int atom, int type);
      int indigoAddStereocenter (int atom, int type, int v1, int v2, int v3, int v4);

      sbyte* indigoSymbol (int atom);
      int indigoDegree (int atom);
      int indigoGetCharge (int atom, int* charge);
      int indigoGetExplicitValence (int atom, int* valence);
      int indigoGetRadicalElectrons (int atom, int* electrons);
      int indigoAtomicNumber (int atom);
      int indigoIsotope (int atom);
      int indigoValence (int atom);
      int indigoCountHydrogens (int atom, int* hydro);
      int indigoCountImplicitHydrogens (int item);

      int indigoCountSuperatoms (int item);
      int indigoCountDataSGroups (int item);
      int indigoCountGenericSGroups (int item);
      int indigoCountRepeatingUnits (int item);
      int indigoCountMultipleGroups (int item);

      int indigoIterateSuperatoms (int item);
      int indigoIterateDataSGroups (int item);
      int indigoIterateGenericSGroups (int item);
      int indigoIterateRepeatingUnits (int item);
      int indigoIterateMultipleGroups (int item);

      int indigoGetDataSGroup (int mol, int idx);
      int indigoGetSuperatom (int mol, int idx);

      sbyte* indigoDescription (int item);
      int indigoAddDataSGroup (int molecule, int natoms, int[] atoms, int nbonds, int[] bonds, string description, string data);
      int indigoSetDataSGroupXY (int sgroup, float x, float y, string options);

      int indigoAddSuperatom (int molecule, int natoms, int []atoms, string name);

      float* indigoXYZ (int atom);
      int indigoSetXYZ (int atom, float x, float y, float z);
      int indigoResetCharge (int atom);
      int indigoResetExplicitValence (int atom);
      int indigoResetRadical (int atom);
      int indigoResetIsotope (int atom);
      int indigoSetAttachmentPoint (int atom, int order);
      int indigoIterateAttachmentPoints (int item, int order);
      int indigoClearAttachmentPoints (int item);
      int indigoRemoveConstraints (int item, string type);
      int indigoAddConstraint (int item, string type, string value);
      int indigoAddConstraintNot (int item, string type, string value);
      int indigoAddConstraintOr (int item, string type, string value);
      int indigoInvertStereo (int atom);
      int indigoResetStereo (int atom);
      int indigoCountAtoms (int molecule);
      int indigoCountBonds (int molecule);
      int indigoCountPseudoatoms (int molecule);
      int indigoCountRSites (int molecule);
      int indigoIterateBonds (int molecule);
      int indigoBondOrder (int molecule);
      int indigoBondStereo (int molecule);
      int indigoTopology (int bond);
      int indigoIterateNeighbors (int atom);
      int indigoBond (int nei);
      int indigoGetAtom (int molecule, int idx);
      int indigoGetBond (int molecule, int idx);
      int indigoGetMolecule (int reaction, int idx);
      int indigoSource (int bond);
      int indigoDestination (int bond);
      int indigoClearCisTrans (int item);
      int indigoClearStereocenters (int item);
      int indigoClearAlleneCenters (int item);
      int indigoCountStereocenters (int item);
      int indigoCountAlleneCenters (int item);
      int indigoResetSymmetricCisTrans (int handle);
      int indigoMarkEitherCisTrans (int handle);

      int indigoAddAtom (int molecule, string symbol);
      int indigoResetAtom (int atom, string symbol);
      int indigoAddRSite (int molecule, string name);
      int indigoSetRSite (int atom, string name);
      int indigoSetCharge (int atom, int charge);
      int indigoSetIsotope (int atom, int isotope);
      int indigoSetImplicitHCount (int atom, int implh);

      int indigoAddBond (int source, int destination, int order);
      int indigoSetBondOrder (int bond, int order);
      int indigoMerge (int where_to, int what);
      int indigoHighlight (int item);
      int indigoUnhighlight (int item);
      int indigoIsHighlighted (int item);

      int indigoCountComponents (int molecule);
      int indigoComponentIndex (int atom);
      int indigoIterateComponents (int molecule);
      int indigoComponent (int molecule, int index);

      int indigoCountSSSR (int molecule);
      int indigoIterateSSSR (int molecule);
      int indigoIterateSubtrees (int molecule, int min_atoms, int max_atoms);
      int indigoIterateRings (int molecule, int min_atoms, int max_atoms);
      int indigoIterateEdgeSubmolecules (int molecule, int min_bonds, int max_bonds);

      int indigoCountHeavyAtoms (int molecule);
      int indigoGrossFormula (int molecule);
      float indigoMolecularWeight (int molecule);
      float indigoMostAbundantMass (int molecule);
      float indigoMonoisotopicMass (int molecule);
      sbyte* indigoCanonicalSmiles (int molecule);
      sbyte* indigoLayeredCode (int molecule);
      int indigoHasCoord (int molecule);
      int indigoHasZCoord (int molecule);
      int indigoIsChiral (int molecule);
      int indigoCreateSubmolecule (int molecule, int nvertices, int[] vertices);
      int indigoCreateEdgeSubmolecule (int molecule, int nvertices, int[] vertices,
                                         int nedges, int[] edges);
      int indigoRemoveAtoms (int molecule, int nvertices, int[] vertices);

      int indigoGetSubmolecule (int molecule, int nvertices, int[] vertices);

      float indigoAlignAtoms (int molecule, int natoms, int[] atom_ids, float[] desired_xyz);
      int indigoAromatize (int item);
      int indigoDearomatize (int item);
      int indigoFoldHydrogens (int item);
      int indigoUnfoldHydrogens (int item);
      int indigoLayout (int item);
      sbyte* indigoSmiles (int item);
      int indigoExactMatch (int item1, int item2, string flags);
      int indigoSetTautomerRule (int id, string beg, string end);
      int indigoRemoveTautomerRule (int id);
      int indigoClearTautomerRules ();
      sbyte* indigoName (int item);
      int indigoSetName (int item, string name);
      int indigoSerialize (int handle, byte** buf, int* size);
      int indigoUnserialize (byte[] buf, int size);
      int indigoHasProperty (int handle, string field);
      sbyte* indigoGetProperty (int handle, string field);
      int indigoSetProperty (int handle, string field, string value);
      int indigoRemoveProperty (int item, string prop);
      int indigoIterateProperties (int handle);
      int indigoClearProperties (int handle);
      sbyte* indigoCheckBadValence (int handle);
      sbyte* indigoCheckAmbiguousH (int handle);

      int indigoFingerprint (int item, string type);
      int indigoCountBits (int fingerprint);
      int indigoCommonBits (int fingerprint1, int fingerprint2);
      float indigoSimilarity (int molecule1, int molecule2, string metrics);

      int indigoIterateSDF (int reader);
      int indigoIterateRDF (int reader);
      int indigoIterateSmiles (int reader);
      int indigoIterateCML (int reader);
      int indigoIterateSDFile (string filename);
      int indigoIterateRDFile (string filename);
      int indigoIterateSmilesFile (string filename);
      int indigoIterateCMLFile (string filename);
      sbyte* indigoRawData (int item);
      int indigoTell (int item);
      int indigoSdfAppend (int output, int item);
      int indigoSmilesAppend (int output, int item);
      int indigoRdfHeader (int output);
      int indigoRdfAppend (int output, int item);
      int indigoCmlHeader (int output);
      int indigoCmlAppend (int output, int item);
      int indigoCmlFooter (int output);

      int indigoCreateArray ();
      int indigoArrayAdd (int arr, int item);
      int indigoAt (int item, int index);
      int indigoCount (int arr);
      int indigoClear (int arr);
      int indigoIterateArray (int arr);

      int indigoSubstructureMatcher (int target, string mode);
      int indigoIgnoreAtom (int matcher, int atom);
      int indigoUnignoreAtom (int matcher, int atom);
      int indigoUnignoreAllAtoms (int matcher);
      int indigoMatch (int matcher, int query);
      int indigoCountMatches (int matcher, int query);
      int indigoCountMatchesWithLimit (int matcher, int query, int embeddings_limit);
      int indigoIterateMatches (int matcher, int query);
      int indigoHighlightedTarget (int match);
      int indigoMapAtom (int match, int query_atom);
      int indigoMapBond (int match, int query_bond);
      int indigoMapMolecule (int match, int query_reaction_molecule);

      int indigoExtractCommonScaffold (int structures, string options);
      int indigoAllScaffolds (int extracted);

      int indigoDecomposeMolecules (int scaffold, int structures);
      int indigoDecomposedMoleculeScaffold (int decomp);
      int indigoIterateDecomposedMolecules (int decomp);
      int indigoDecomposedMoleculeHighlighted (int decomp);
      int indigoDecomposedMoleculeSubstituents (int decomp);
      int indigoDecomposedMoleculeWithRGroups (int decomp);
      int indigoCreateDecomposer(int scaffold);
      int indigoDecomposeMolecule(int decomp, int mol);
      int indigoIterateDecompositions(int deco_item);
      int indigoAddDecomposition(int decomp, int q_match);

      int indigoNext (int iter);
      int indigoHasNext (int iter);
      int indigoIndex (int item);
      int indigoRemove (int item);

      sbyte* indigoToString (int handle);
      int indigoToBuffer (int handle, byte** buf, int* size);

      int indigoReactionProductEnumerate (int reaction, int monomers);
      int indigoTransform (int reaction, int monomers);

      sbyte* indigoDbgInternalType (int item);
	}
}
