﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace com.ggasoftware.indigo
{
   public unsafe class IndigoObject : IEnumerable, IDisposable
   {
      public int self;
      private Indigo dispatcher;
      private object parent; // to prevent GC killing the parent object
      private IndigoLib _indigo_lib;

      public IndigoObject (Indigo dispatcher, int id) : this(dispatcher, null, id)
      {
      }

      public IndigoObject (Indigo dispatcher, object parent, int id)
      {
         this.dispatcher = dispatcher;
         this.self = id;
         this.parent = parent;
         _indigo_lib = dispatcher._indigo_lib;
      }

      ~IndigoObject ()
      {
         Dispose();
      }

      public void Dispose ()
      {
         if (dispatcher == null)
         {
            // This happens exclusively in 32-bit .NET environment
            // after an IndigoObject constructor throws an exception.
            // In fact, the object is not created in this case,
            // but for some reason the .NET VM disposes it, despite it
            // has not been initialized.
            return;
         }
         if (self >= 0)
         {
            // Check that the session is still alive
            // (.NET has no problem disposing referenced
            // objects before the objects that reference to them)
            if (dispatcher.getSID() >= 0)
            {
               dispatcher.setSessionID();
               dispatcher.free(self);
               self = -1;
            }
         }
      }

      public IndigoObject clone ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, _indigo_lib.indigoClone(self));
      }

      public void close ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoClose(self);
      }

      public String molfile ()
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoMolfile(self));
      }

      public void saveMolfile (String filename)
      {
         dispatcher.setSessionID();
         int s = _indigo_lib.indigoWriteFile(filename);
         _indigo_lib.indigoSaveMolfile(self, s);
         _indigo_lib.indigoFree(s);
      }

      public String cml ()
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoCml(self));
      }

      public void saveCml (String filename)
      {
         dispatcher.setSessionID();
         int s = _indigo_lib.indigoWriteFile(filename);
         _indigo_lib.indigoSaveCml(self, s);
         _indigo_lib.indigoFree(s);
      }

      public byte[] mdlct ()
      {
         dispatcher.setSessionID();
         IndigoObject buf = dispatcher.writeBuffer();
         _indigo_lib.indigoSaveMDLCT(self, buf.self);
         return buf.toBuffer();
      }

      public void addReactant (IndigoObject molecule)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoAddReactant(self, molecule.self);
      }

      public void addProduct (IndigoObject molecule)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoAddProduct(self, molecule.self);
      }

      public void addCatalyst (IndigoObject molecule)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoAddCatalyst(self, molecule.self);
      }

      public int countReactants ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountReactants(self);
      }

      public int countProducts ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountProducts(self);
      }

      public int countCatalysts ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountCatalysts(self);
      }

      public int countMolecules ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountMolecules(self);
      }

      public System.Collections.IEnumerable iterateReactants ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateReactants(self));
      }

      public System.Collections.IEnumerable iterateProducts ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateProducts(self));
      }

      public System.Collections.IEnumerable iterateCatalysts ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateCatalysts(self));
      }

      public System.Collections.IEnumerable iterateMolecules ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateMolecules(self));
      }

      public String rxnfile ()
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoRxnfile(self));
      }

      public void saveRxnfile (String filename)
      {
         dispatcher.setSessionID();
         int s = _indigo_lib.indigoWriteFile(filename);
         _indigo_lib.indigoSaveRxnfile(self, s);
         _indigo_lib.indigoFree(s);
      }

      public void automap (String mode)
      {
         if (mode == null)
            mode = "";
         dispatcher.setSessionID();
         _indigo_lib.indigoAutomap(self, mode);
      }

      public int atomMappingNumber (IndigoObject reaction_atom)
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoGetAtomMappingNumber(self, reaction_atom.self);
      }

      public void setAtomMappingNumber (IndigoObject reaction_atom, int number)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoSetAtomMappingNumber(self, reaction_atom.self, number);
      }

      public ReactingCenter reactingCenter (IndigoObject bond)
      {
         int c;
         dispatcher.setSessionID();

         if (_indigo_lib.indigoGetReactingCenter(self, bond.self, &c) == 1)
            return (ReactingCenter)c;
         throw new IndigoException("reactingCenter(): unexpected result");
      }

      public void setReactingCenter (IndigoObject bond, ReactingCenter type)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoSetReactingCenter(self, bond.self, (int)type);
      }

      public void clearAAM ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoClearAAM(self);
      }

      public void correctReactingCenters ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoCorrectReactingCenters(self);
      }
   
      public System.Collections.IEnumerable iterateAtoms ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateAtoms(self));
      }

      public System.Collections.IEnumerable iteratePseudoatoms ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIteratePseudoatoms(self));
      }

      public System.Collections.IEnumerable iterateRSites ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateRSites(self));
      }

      public System.Collections.IEnumerable iterateStereocenters ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateStereocenters(self));
      }

      public System.Collections.IEnumerable iterateAlleneCenters ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateAlleneCenters(self));
      }

      public System.Collections.IEnumerable iterateRGroups ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateRGroups(self));
      }

      public System.Collections.IEnumerable iterateRGroupFragments ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateRGroupFragments(self));
      }

      public int countAttachmentPoints ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountAttachmentPoints(self);
      }

      public bool isPseudoatom ()
      {
         dispatcher.setSessionID();
         if (_indigo_lib.indigoIsPseudoatom(self) == 1)
            return true;
         return false;
      }

      public bool isRSite ()
      {
         dispatcher.setSessionID();
         if (_indigo_lib.indigoIsRSite(self) == 1)
            return true;
         return false;
      }

      public int stereocenterType ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoStereocenterType(self);
      }

      public void changeStereocenterType (int type)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoChangeStereocenterType(self, type);
      }

      public int singleAllowedRGroup ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoSingleAllowedRGroup(self);
      }

      public string symbol ()
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoSymbol(self));
      }

      public int degree ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoDegree(self);
      }

      public int? charge ()
      {
         int c;
         dispatcher.setSessionID();

         if (_indigo_lib.indigoGetCharge(self, &c) == 1)
            return c;
         return null;
      }

      public int? explicitValence ()
      {
         int c;
         dispatcher.setSessionID();

         if (_indigo_lib.indigoGetExplicitValence(self, &c) == 1)
            return c;
         return null;
      }

      public int? radicalElectrons ()
      {
         int c;
         dispatcher.setSessionID();

         if (_indigo_lib.indigoGetRadicalElectrons(self, &c) == 1)
            return c;
         return null;
      }

      public int atomicNumber ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoAtomicNumber(self);
      }

      public int isotope ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoIsotope(self);
      }

      public int valence ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoValence(self);
      }

      public int? countHydrogens ()
      {
         int h;
         dispatcher.setSessionID();

         if (_indigo_lib.indigoCountHydrogens(self, &h) == 1)
            return h;
         return null;
      }

      public int countImplicitHydrogens ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountImplicitHydrogens(self);
      }

      public int countSuperatoms ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountSuperatoms(self);
      }

      public int countDataSGroups ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountDataSGroups(self);
      }

      public int countGenericSGroups ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountGenericSGroups(self);
      }

      public int countRepeatingUnits ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountRepeatingUnits(self);
      }

      public int countMultipleGroups ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountMultipleGroups(self);
      }

      public IndigoObject iterateSuperatoms ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateSuperatoms(self));
      }

      public IndigoObject iterateAttachmentPoints (int order)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateAttachmentPoints(self, order));
      }

      public IndigoObject iterateDataSGroups ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateDataSGroups(self));
      }

      public IndigoObject iterateGenericSGroups ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateGenericSGroups(self));
      }

      public IndigoObject iterateRepeatingUnits ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateRepeatingUnits(self));
      }

      public IndigoObject iterateMultipleGroups ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateMultipleGroups(self));
      }

      public IndigoObject getDataSGroup (int index)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoGetDataSGroup(self, index));
      }

      public IndigoObject getSuperatom (int index)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoGetSuperatom(self, index));
      }

      public string description ()
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoDescription(self));
      }

      public IndigoObject addDataSGroup (int[] atoms, int[] bonds, String description, String data)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, _indigo_lib.indigoAddDataSGroup(self, atoms.Length, atoms, bonds.Length, bonds, description, data));
      }

      public IndigoObject addDataSGroup (ICollection atoms, ICollection bonds, String description, String data)
      {
         return addDataSGroup(Indigo.toIntArray(atoms), Indigo.toIntArray(bonds), description, data);
      }

      public IndigoObject addSuperatom (int[] atoms, String name)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, _indigo_lib.indigoAddSuperatom(self, atoms.Length, atoms, name));
      }

      public IndigoObject addSuperatom (ICollection atoms, String name)
      {
         return addSuperatom(Indigo.toIntArray(atoms), name);
      }

      public void addStereocenter (int type, int v1, int v2, int v3)
      {
         addStereocenter(type, v1, v2, v3, -1);
      }

      public void addStereocenter (int type, int v1, int v2, int v3, int v4)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoAddStereocenter(self, type, v1, v2, v3, v4);
      }

      public void setDataSGroupXY (float x, float y, String options)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoSetDataSGroupXY(self, x, y, options);
      }

      public float[] xyz ()
      {
         dispatcher.setSessionID();
         float *ptr = _indigo_lib.indigoXYZ(self);
         float[] res = new float[3];
         res[0] = ptr[0];
         res[1] = ptr[1];
         res[2] = ptr[2];
         return res;
      }

      public void setXYZ (float x, float y, float z)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoSetXYZ(self, x, y, z);
      }

      public void resetCharge ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoResetCharge(self);
      }

      public void resetExplicitValence ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoResetExplicitValence(self);
      }

      public void resetRadical ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoResetRadical(self);
      }

      public void resetIsotope ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoResetIsotope(self);
      }

      public void setAttachmentPoint (int order)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoSetAttachmentPoint(self, order);
      }

      public void clearAttachmentPoints ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoClearAttachmentPoints(self);
      }

      public void removeConstraints (string type)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoRemoveConstraints(self, type);
      }

      public void addConstraint (string type, string value)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoAddConstraint(self, type, value);
      }

      public void addConstraintNot (string type, string value)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoAddConstraintNot(self, type, value);
      }

      public void addConstraintOr (string type, string value)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoAddConstraintOr(self, type, value);
      }

      public void invertStereo ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoInvertStereo(self);
      }

      public void resetStereo ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoResetStereo(self);
      }

      public int countAtoms ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountAtoms(self);
      }

      public int countBonds ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountBonds(self);
      }

      public int countPseudoatoms ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountPseudoatoms(self);
      }

      public int countRSites ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountRSites(self);
      }

      public System.Collections.IEnumerable iterateBonds ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateBonds(self));
      }

      public int bondOrder ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoBondOrder(self);
      }

      public int bondStereo ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoBondStereo(self);
      }

      public int topology ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoTopology(self);
      }

      public System.Collections.IEnumerable iterateNeighbors ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateNeighbors(self));
      }

      public IndigoObject bond ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, _indigo_lib.indigoBond(self));
      }

      public IndigoObject getAtom (int idx)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoGetAtom(self, idx));
      }

      public IndigoObject getBond (int idx)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoGetBond(self, idx));
      }

      public IndigoObject getMolecule (int idx)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoGetMolecule(self, idx));
      }

      public IndigoObject source ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, _indigo_lib.indigoSource(self));
      }

      public IndigoObject destination ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, _indigo_lib.indigoDestination(self));
      }

      public void clearCisTrans ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoClearCisTrans(self);
      }

      public void clearStereocenters()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoClearStereocenters(self);
      }

      public void clearAlleneCenters ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoClearAlleneCenters(self);
      }

      public int countStereocenters ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountStereocenters(self);
      }

      public int countAlleneCenters ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountAlleneCenters(self);
      }

      public int resetSymmetricCisTrans ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoResetSymmetricCisTrans(self);
      }

      public int markEitherCisTrans ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoMarkEitherCisTrans(self);
      }

      public IndigoObject addAtom (string symbol)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoAddAtom(self, symbol));
      }

      public IndigoObject resetAtom (string symbol)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoResetAtom(self, symbol));
      }

      public IndigoObject addRSite (string name)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoAddRSite(self, name));
      }

      public IndigoObject setRSite (string name)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoSetRSite(self, name));
      }

      public void setCharge (int charge)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoSetCharge(self, charge);
      }

      public void setIsotope (int isotope)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoSetIsotope(self, isotope);
      }

      public void setImplicitHCount (int implh)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoSetImplicitHCount(self, implh);
      }

      public IndigoObject addBond (IndigoObject dest, int order)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoAddBond(self, dest.self, order));
      }

      public void setBondOrder (int order)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoSetBondOrder(self, order);
      }

      public IndigoObject merge (IndigoObject what)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoMerge(self, what.self));
      }

      public void highlight ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoHighlight(self);
      }

      public void unhighlight ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoUnhighlight(self);
      }

      public bool isHighlighted ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoIsHighlighted(self) == 1;
      }

      public int countComponents ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountComponents(self);
      }

      public int componentIndex ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoComponentIndex(self);
      }

      public void remove ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoRemove(self);
      }

      public IndigoObject iterateComponents ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateComponents(self));
      }

      public IndigoObject component (int index)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoComponent(self, index));
      }

      public int countSSSR ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountSSSR(self);
      }

      public IndigoObject iterateSSSR ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateSSSR(self));
      }

      public IndigoObject iterateSubtrees (int min_vertices, int max_vertices)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateSubtrees(self, min_vertices, max_vertices));
      }

      public IndigoObject iterateRings (int min_vertices, int max_vertices)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateRings(self, min_vertices, max_vertices));
      }

      public IndigoObject iterateEdgeSubmolecules (int min_edges, int max_edges)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateEdgeSubmolecules(self, min_edges, max_edges));
      }

      public int countHeavyAtoms ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountHeavyAtoms(self);
      }

      public String grossFormula ()
      {
         dispatcher.setSessionID();
         int gf = _indigo_lib.indigoGrossFormula(self);
         String result = new String(_indigo_lib.indigoToString(gf));
         _indigo_lib.indigoFree(gf);
         return result;
      }

      public float molecularWeight ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoMolecularWeight(self);
      }

      public float mostAbundantMass ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoMostAbundantMass(self);
      }

      public float monoisotopicMass ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoMonoisotopicMass(self);
      }

      public string canonicalSmiles ()
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoCanonicalSmiles(self));
      }

      public string layeredCode ()
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoLayeredCode(self));
      }

      public bool hasCoord ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoHasCoord(self) == 1;
      }

      public bool hasZCoord ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoHasZCoord(self) == 1;
      }

      public bool isChiral ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoIsChiral(self) == 1;
      }

      public IndigoObject createSubmolecule (int[] vertices)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, _indigo_lib.indigoCreateSubmolecule(self, vertices.Length, vertices));
      }

      public IndigoObject createSubmolecule (ICollection vertices)
      {
         return createSubmolecule(Indigo.toIntArray(vertices));
      }

      public IndigoObject getSubmolecule (int[] vertices)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, _indigo_lib.indigoGetSubmolecule(self, vertices.Length, vertices));
      }

      public IndigoObject getSubmolecule (ICollection vertices)
      {
         return getSubmolecule(Indigo.toIntArray(vertices));
      }

      public IndigoObject createEdgeSubmolecule (int[] vertices, int[] edges)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, _indigo_lib.indigoCreateEdgeSubmolecule(self,
            vertices.Length, vertices, edges.Length, edges));
      }

      public IndigoObject createEdgeSubmolecule (ICollection vertices, ICollection edges)
      {
         return createEdgeSubmolecule(vertices, edges);
      }

      public void removeAtoms (int[] vertices)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoRemoveAtoms(self, vertices.Length, vertices);
      }

      public void removeAtoms (ICollection vertices)
      {
         removeAtoms(Indigo.toIntArray(vertices));
      }

      public float alignAtoms (int[] atom_ids, float[] desired_xyz)
      {
         dispatcher.setSessionID();
         if (atom_ids.Length * 3 != desired_xyz.Length)
            throw new IndigoException("alignAtoms(): desired_xyz[] must be exactly 3 times bigger than atom_ids[]");
         return _indigo_lib.indigoAlignAtoms(self, atom_ids.Length, atom_ids, desired_xyz);
      }

      public float alignAtoms (ICollection atom_ids, ICollection desired_xyz)
      {
         return alignAtoms(Indigo.toIntArray(atom_ids), Indigo.toFloatArray(desired_xyz));
      }

      public void aromatize ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoAromatize(self);
      }

      public void dearomatize ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoDearomatize(self);
      }

      public void foldHydrogens ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoFoldHydrogens(self);
      }

      public void unfoldHydrogens ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoUnfoldHydrogens(self);
      }

      public void layout ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoLayout(self);
      }

      public string smiles ()
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoSmiles(self));
      }

      public String name ()
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoName(self));
      }

      public void setName (string name)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoSetName(self, name);
      }

      public byte[] serialize ()
      {
         dispatcher.setSessionID();
         byte* buf;
         int bufsize;
         _indigo_lib.indigoSerialize(self, &buf, &bufsize);

         byte[] res = new byte[bufsize];
         for (int i = 0; i < bufsize; ++i)
            res[i] = buf[i];
         return res;
      }

      public bool hasProperty (string name)
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoHasProperty(self, name) == 1;
      }

      public string getProperty (string name)
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoGetProperty(self, name));
      }

      public void setProperty (string name, string value)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoSetProperty(self, name, value);
      }

      public void removeProperty (string name)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoRemoveProperty(self, name);
      }

      public System.Collections.IEnumerable iterateProperties ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateProperties(self));
      }

      public void clearProperties ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoClearProperties(self);
      }

      public string checkBadValence ()
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoCheckBadValence(self));
      }

      public string checkAmbiguousH ()
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoCheckAmbiguousH(self));
      }

      public IndigoObject fingerprint (string type)
      {
         dispatcher.setSessionID();
         if (type == null)
            type = "";
         return new IndigoObject(dispatcher, _indigo_lib.indigoFingerprint(self, type));
      }

      public int countBits ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountBits(self);
      }

      public string rawData ()
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoRawData(self));
      }

      public int tell ()
      {

         dispatcher.setSessionID();
         return _indigo_lib.indigoTell(self);
      }

      public void sdfAppend (IndigoObject item)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoSdfAppend(self, item.self);
      }

      public void smilesAppend (IndigoObject item)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoSmilesAppend(self, item.self);
      }

      public void rdfHeader ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoRdfHeader(self);
      }

      public void rdfAppend (IndigoObject item)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoRdfAppend(self, item.self);
      }

      public void cmlHeader ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoCmlHeader(self);
      }

      public void cmlAppend (IndigoObject item)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoCmlAppend(self, item.self);
      }

      public void cmlFooter ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoCmlFooter(self);
      }

      public int arrayAdd (IndigoObject item)
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoArrayAdd(self, item.self);
      }

      public IndigoObject at (int index)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoAt(self, index));
      }

      public int count ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCount(self);
      }

      public void clear ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoClear(self);
      }

      public System.Collections.IEnumerable iterateArray ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateArray(self));
      }

      public void ignoreAtom (IndigoObject atom)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoIgnoreAtom(self, atom.self);
      }

      public void unignoreAtom (IndigoObject atom)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoUnignoreAtom(self, atom.self);
      }

      public void unignoreAllAtoms ()
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoUnignoreAllAtoms(self);
      }

      public IndigoObject match (IndigoObject query)
      {
         dispatcher.setSessionID();
         int res = _indigo_lib.indigoMatch(self, query.self);
         if (res == 0)
            return null;
         return new IndigoObject(dispatcher, this, res);
      }

      public int countMatches (IndigoObject query)
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountMatches(self, query.self);
      }

      public int countMatchesWithLimit (IndigoObject query, int embeddings_limit)
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoCountMatchesWithLimit(self, query.self, embeddings_limit);
      }

      public System.Collections.IEnumerable iterateMatches (IndigoObject query)
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateMatches(self, query.self));
      }

      public IndigoObject highlightedTarget ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, _indigo_lib.indigoHighlightedTarget(self));
      }

      public IndigoObject mapAtom (IndigoObject query_atom)
      {
         dispatcher.setSessionID();
         int mapped = _indigo_lib.indigoMapAtom(self, query_atom.self);
         if (mapped == 0)
            return null;
         return new IndigoObject(dispatcher, mapped);
      }

      public IndigoObject mapMolecule (IndigoObject query_reaction_molecule)
      {
         dispatcher.setSessionID();
         int mapped = _indigo_lib.indigoMapMolecule(self, query_reaction_molecule.self);
         if (mapped == 0)
            return null;
         return new IndigoObject(dispatcher, mapped);
      }

      public IndigoObject mapBond (IndigoObject query_bond)
      {
         dispatcher.setSessionID();
         int mapped = _indigo_lib.indigoMapBond(self, query_bond.self);
         if (mapped == 0)
            return null;
         return new IndigoObject(dispatcher, mapped);
      }

      public IndigoObject allScaffolds ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, _indigo_lib.indigoAllScaffolds(self));
      }

      public IndigoObject decomposedMoleculeScaffold ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, _indigo_lib.indigoDecomposedMoleculeScaffold(self));
      }

      public System.Collections.IEnumerable iterateDecomposedMolecules ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, this, _indigo_lib.indigoIterateDecomposedMolecules(self));
      }

      public IndigoObject decomposedMoleculeHighlighted ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, _indigo_lib.indigoDecomposedMoleculeHighlighted(self));
      }

      public IndigoObject decomposedMoleculeWithRGroups ()
      {
         dispatcher.setSessionID();
         return new IndigoObject(dispatcher, _indigo_lib.indigoDecomposedMoleculeWithRGroups(self));
      }

      public IndigoObject decomposeMolecule(IndigoObject mol)
      {
          dispatcher.setSessionID();
          int res =  _indigo_lib.indigoDecomposeMolecule(self, mol.self);
          if (res == 0)
              return null;
          return new IndigoObject(dispatcher, this, res);
      }

      public System.Collections.IEnumerable iterateDecompositions()
      {
          dispatcher.setSessionID();
          int res = _indigo_lib.indigoIterateDecompositions(self);
          if (res == 0)
              return null;
          return new IndigoObject(dispatcher, this, res);
      }

      public void addDecomposition(IndigoObject q_match)
      {
          dispatcher.setSessionID();
          _indigo_lib.indigoAddDecomposition(self, q_match.self);
      }
      public IEnumerator GetEnumerator ()
      {
         while (true)
         {
            dispatcher.setSessionID();
            int next = _indigo_lib.indigoNext(self);
            if (next == 0)
               break;
            yield return new IndigoObject(dispatcher, this, next);
         }
      }

      public IndigoObject next ()
      {
         dispatcher.setSessionID();
         int next = _indigo_lib.indigoNext(self);
         if (next == 0)
            return null;
         return new IndigoObject(dispatcher, this, next);
      }

      public bool hasNext ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoHasNext(self) == 1;
      }

      public int index ()
      {
         dispatcher.setSessionID();
         return _indigo_lib.indigoIndex(self);
      }

      public String toString ()
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoToString(self));
      }

      public byte[] toBuffer ()
      {
         dispatcher.setSessionID();
         byte* buf;
         int bufsize;
         _indigo_lib.indigoToBuffer(self, &buf, &bufsize);

         byte[] res = new byte[bufsize];
         for (int i = 0; i < bufsize; ++i)
            res[i] = buf[i];
         return res;
      }

      public void append (IndigoObject obj)
      {
         dispatcher.setSessionID();
         _indigo_lib.indigoAppend(self, obj.self);
      }

      public void optimize ()
      {
         optimize(null);
      }

      public void optimize (string options)
      {
         dispatcher.setSessionID();
         if (options == null)
            options = "";
         _indigo_lib.indigoOptimize(self, options);
      }

      public String dbgInternalType ()
      {
         dispatcher.setSessionID();
         return new String(_indigo_lib.indigoDbgInternalType(self));
      }
   }
}
